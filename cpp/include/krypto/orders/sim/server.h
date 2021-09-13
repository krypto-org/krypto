#pragma once

#include <krypto/orders/protocol.h>
#include <krypto/mktdata/protocol.h>
#include <krypto/network/subscriber.h>
#include <krypto/network/rpc/worker.h>
#include <krypto/network/publisher.h>

#include <tbb/concurrent_hash_map.h>
#include <krypto/serialization/helper.h>
#include <krypto/utils/types.h>
#include <krypto/utils/date_time.h>

namespace krypto::orders::sim {
    template<krypto::serialization::Exchange Exchange>
    class OrderServer final {
    private:

        std::unique_ptr<zmq::socket_t> receiver_;
        std::unique_ptr<zmq::socket_t> mktdata_subscriber_;
        std::string order_gateway_endpoint_;
        std::string mktdata_gateway_endpoint_;
        std::atomic_bool running_;
        std::unordered_map<std::string, krypto::utils::MsgType> msg_type_ref_;
        flatbuffers::FlatBufferBuilder fb_builder_;
        std::unordered_map<int64_t, krypto::utils::Quote> quotes_;
        std::unordered_map<std::string, int64_t> order_id_to_security_id_map_;
        std::unordered_map<std::string, std::string> order_id_to_client_identity_map_;
        std::unordered_map<int64_t, std::unordered_map<
                std::string,
                krypto::utils::OrderRequest>> day_orders_;
        std::unordered_map<int64_t, uint64_t> last_hb_;

        void serialize(const krypto::utils::OrderUpdate &);

        void fill_price(int64_t security_id, const krypto::serialization::Side &, int64_t price);

        void send(const krypto::utils::OrderUpdate &order_update,
                  const std::string &client_identity);

        void send_reject(krypto::serialization::OrderStatus, const std::string &order_id,
                         const std::string &client_identity);

        void send_ready(const std::string &identity);

        void send_disconnect(const std::string &identity);

    public:
        OrderServer(zmq::context_t &, const krypto::Config &);

        void process(const krypto::serialization::OrderRequest *, const std::string &client_identity);

        void process(const krypto::serialization::OrderCancelRequest *, const std::string &client_identity);

        void process(const krypto::serialization::OrderReplaceRequest *, const std::string &client_identity);

        void process(const krypto::serialization::Quote *quote);

        void process(const krypto::serialization::Trade *trade);

        void process(const krypto::serialization::Heartbeat *hb);

        void start();

        void stop();
    };

    template<krypto::serialization::Exchange Exchange>
    OrderServer<Exchange>::OrderServer(
            zmq::context_t &context, const krypto::Config &config) :
            receiver_{std::make_unique<zmq::socket_t>(context, ZMQ_DEALER)},
            mktdata_subscriber_{std::make_unique<zmq::socket_t>(context, ZMQ_SUB)},
            order_gateway_endpoint_{config.at<std::string>(
                    "/services/order_gateway/backend/client")},
            mktdata_gateway_endpoint_{config.at<std::string>(
                    "/services/mktdata_gateway/backend/client")},
            running_{false},
            msg_type_ref_{krypto::utils::name_to_msg_type()} {
    }

    template<krypto::serialization::Exchange Exchange>
    void OrderServer<Exchange>::serialize(const krypto::utils::OrderUpdate &order_update) {
        krypto::serialization::serialize(fb_builder_, order_update);
    }

    template<krypto::serialization::Exchange Exchange>
    void OrderServer<Exchange>::fill_price(
            int64_t security_id, const krypto::serialization::Side &side,
            int64_t price) {

        if (day_orders_.find(security_id) == std::end(day_orders_)) {
            return;
        }

        auto &orders = day_orders_.at(security_id);

        std::vector<std::string> filled_order_ids;

        std::for_each(std::begin(orders), std::end(orders), [&](auto &&pair) {
            if (side == krypto::serialization::Side::Side_BUY &&
                pair.second.side == krypto::serialization::Side::Side_SELL) {
                if (pair.second.price <= price) {
                    auto order_id = pair.second.order_id;
                    KRYP_LOG(info, "=> {} -- filled", order_id);
                    const krypto::utils::OrderUpdate fill{
                            krypto::utils::current_time_in_nanoseconds(),
                            order_id,
                            order_id,
                            krypto::serialization::OrderStatus::OrderStatus_FILLED,
                            pair.second.quantity
                    };
                    send(fill, order_id_to_client_identity_map_.at(order_id));
                    filled_order_ids.push_back(order_id);
                }
            } else if (side == krypto::serialization::Side::Side_SELL &&
                       pair.second.side == krypto::serialization::Side::Side_BUY) {
                if (pair.second.price >= price) {
                    auto order_id = pair.second.order_id;
                    KRYP_LOG(info, "=> {} -- filled", order_id);
                    const krypto::utils::OrderUpdate fill{
                            krypto::utils::current_time_in_nanoseconds(),
                            order_id,
                            order_id,
                            krypto::serialization::OrderStatus::OrderStatus_FILLED,
                            pair.second.quantity
                    };
                    send(fill, order_id_to_client_identity_map_.at(order_id));
                    filled_order_ids.push_back(order_id);
                }
            }
        });

        for (auto && order_id : filled_order_ids) {
            day_orders_.at(security_id).erase(order_id);
            order_id_to_client_identity_map_.erase(order_id);
            order_id_to_security_id_map_.equal_range(order_id);
        }
    }

    template<krypto::serialization::Exchange Exchange>
    void OrderServer<Exchange>::process(const krypto::serialization::Quote *quote) {
        quotes_[quote->security_id()] = krypto::utils::Quote{
                static_cast<int64_t >(quote->timestamp()),
                static_cast<int64_t >(quote->security_id()),
                quote->bid(),
                quote->ask(),
                quote->bid_quantity(),
                quote->ask_quantity(),
                quote->last(),
                quote->last_quantity()
        };
    }

    template<krypto::serialization::Exchange Exchange>
    void OrderServer<Exchange>::process(const krypto::serialization::OrderRequest *request,
                                                 const std::string &client_identity) {
        KRYP_LOG(info, "Processing new order request : {}", request->order_id()->str());
        const krypto::utils::OrderUpdate in_flight{
                krypto::utils::current_time_in_nanoseconds(),
                request->order_id()->str(),
                request->order_id()->str(),
                krypto::serialization::OrderStatus::OrderStatus_IN_FLIGHT,
                0};
        send(in_flight, client_identity);

        const krypto::utils::OrderUpdate accept{
                krypto::utils::current_time_in_nanoseconds(),
                request->order_id()->str(),
                request->order_id()->str(),
                krypto::serialization::OrderStatus::OrderStatus_ACCEPTED,
                0
        };
        send(accept, client_identity);

        if (quotes_.find(request->security_id()) != std::end(quotes_)) {
            if ((request->side() == krypto::serialization::Side_BUY &&
                 request->price() >= quotes_.at(request->security_id()).ask) ||
                (request->side() == krypto::serialization::Side_SELL &&
                 request->price() <= quotes_.at(request->security_id()).bid)) {

                KRYP_LOG(info, "=> {} -- filled", request->order_id()->str());
                const krypto::utils::OrderUpdate fill{
                        krypto::utils::current_time_in_nanoseconds(),
                        request->order_id()->str(),
                        request->order_id()->str(),
                        krypto::serialization::OrderStatus::OrderStatus_FILLED,
                        request->quantity()
                };
                send(fill, client_identity);
                return;
            } else {
                if (request->tif() == krypto::serialization::TimeInForce::TimeInForce_IOC) {
                    const krypto::utils::OrderUpdate expired{
                            krypto::utils::current_time_in_nanoseconds(),
                            request->order_id()->str(),
                            request->order_id()->str(),
                            krypto::serialization::OrderStatus::OrderStatus_EXPIRED,
                            0
                    };
                    send(expired, client_identity);
                    return;
                } else {
                    if (day_orders_.find(request->security_id()) == std::end(day_orders_)) {
                        day_orders_[request->security_id()] = {};
                    }

                    krypto::utils::OrderRequest r{
                            request->timestamp(),
                            request->security_id(),
                            request->price(),
                            request->quantity(),
                            request->side(),
                            request->order_id()->str(),
                            request->tif()
                    };

                    day_orders_.at(request->security_id())[request->order_id()->str()] = r;
                    order_id_to_security_id_map_[request->order_id()->str()] = request->security_id();
                    order_id_to_client_identity_map_[request->order_id()->str()] = client_identity;

                    const krypto::utils::OrderUpdate new_order{
                            krypto::utils::current_time_in_nanoseconds(),
                            request->order_id()->str(),
                            request->order_id()->str(),
                            krypto::serialization::OrderStatus::OrderStatus_NEW,
                            0
                    };
                    send(new_order, client_identity);
                    return;
                }
            }
        }

        send_reject(
                krypto::serialization::OrderStatus::OrderStatus_REJECTED,
                request->order_id()->str(), client_identity);
    }

    template<krypto::serialization::Exchange Exchange>
    void OrderServer<Exchange>::process(
            const krypto::serialization::OrderCancelRequest *request,
            const std::string &client_identity) {
        KRYP_LOG(info, "Processing cancel order request : {}", request->order_id()->str());
        const krypto::utils::OrderUpdate in_flight{
                krypto::utils::current_time_in_nanoseconds(),
                request->order_id()->str(),
                request->order_id()->str(),
                krypto::serialization::OrderStatus::OrderStatus_CANCEL_IN_FLIGHT,
                0};
        send(in_flight, client_identity);

        if (order_id_to_security_id_map_.find(request->order_id()->str()) ==
            std::cend(order_id_to_security_id_map_)) {
            KRYP_LOG(info, "Order id {} not found", request->order_id()->str());
            send_reject(krypto::serialization::OrderStatus::OrderStatus_CANCEL_REJECTED,
                        request->order_id()->str(), client_identity);
            return;
        }

        const int64_t security_id = order_id_to_security_id_map_.at(request->order_id()->str());

        day_orders_.at(security_id).erase(request->order_id()->str());
        const krypto::utils::OrderUpdate cancelled{
                krypto::utils::current_time_in_nanoseconds(),
                request->order_id()->str(),
                request->order_id()->str(),
                krypto::serialization::OrderStatus::OrderStatus_CANCELLED,
                0};
        send(cancelled, client_identity);
    }

    template<krypto::serialization::Exchange Exchange>
    void OrderServer<Exchange>::process(
            const krypto::serialization::OrderReplaceRequest *request,
            const std::string &client_identity) {
        KRYP_LOG(info, "Processing replace order request : {}", request->order_id()->str());
        const krypto::utils::OrderUpdate in_flight{
                krypto::utils::current_time_in_nanoseconds(),
                request->order_id()->str(),
                request->order_id()->str(),
                krypto::serialization::OrderStatus::OrderStatus_REPLACE_IN_FLIGHT,
                0};
        send(in_flight, client_identity);

        if (order_id_to_security_id_map_.find(request->order_id()->str()) ==
            std::cend(order_id_to_security_id_map_)) {
            KRYP_LOG(info, "Order id {} not found", request->order_id()->str());
            send_reject(krypto::serialization::OrderStatus::OrderStatus_REPLACE_REJECTED,
                        request->order_id()->str(), client_identity);
            return;
        }

        const int64_t security_id = order_id_to_security_id_map_.at(request->order_id()->str());

        day_orders_.at(security_id).at(request->order_id()->str()).price = request->price();
        day_orders_.at(security_id).at(request->order_id()->str()).quantity = request->quantity();
        day_orders_.at(security_id).at(request->order_id()->str()).side = request->side();

        const krypto::utils::OrderUpdate replaced{
                krypto::utils::current_time_in_nanoseconds(),
                request->order_id()->str(),
                request->order_id()->str(),
                krypto::serialization::OrderStatus::OrderStatus_REPLACED,
                request->quantity()};
        send(replaced, client_identity);
    }

    template<krypto::serialization::Exchange Exchange>
    void OrderServer<Exchange>::process(const krypto::serialization::Trade *trade) {
        fill_price(trade->security_id(), trade->side(), trade->price());
    }

    template<krypto::serialization::Exchange Exchange>
    void OrderServer<Exchange>::process(const krypto::serialization::Heartbeat *hb) {
        last_hb_[hb->security_id()] = hb->timestamp();
    }

    template<krypto::serialization::Exchange Exchange>
    void OrderServer<Exchange>::start() {
        auto identity = krypto::serialization::EnumNameExchange(Exchange);
        krypto::network::connect(*receiver_, order_gateway_endpoint_, identity);
        KRYP_LOG(info, "Connecting to market data gateway @ {}", mktdata_gateway_endpoint_);
        mktdata_subscriber_->connect(mktdata_gateway_endpoint_);

        KRYP_LOG(info, "Connected to gateway backend @ {}", order_gateway_endpoint_);

        send_ready(identity);

        KRYP_LOG(info, "Sent ready status to broker @ {}", order_gateway_endpoint_);

        zmq::pollitem_t items[] = {
                {*mktdata_subscriber_, 0, ZMQ_POLLIN, 0},
                {*receiver_,           0, ZMQ_POLLIN, 0}
        };

        running_ = true;

        mktdata_subscriber_->template set(zmq::sockopt::subscribe, "");

        while (running_) {
            zmq::poll(&items[0], 2, 0);
            if (items[0].revents && ZMQ_POLLIN) {

                auto topic = krypto::network::recv_string(*mktdata_subscriber_);
                auto topic_prefix = topic.substr(0, 2);
                auto msg_type = msg_type_ref_.at(topic_prefix);

                zmq::message_t payload_msg;
                const auto payload_size = mktdata_subscriber_->recv(payload_msg, zmq::recv_flags::none);

                if (!payload_size.has_value()) {
                    KRYP_LOG(error, "Payload has 0 size");
                }

                switch (msg_type) {
                    case krypto::utils::MsgType::QUOTE: {
                        process(flatbuffers::GetRoot<krypto::serialization::Quote>(payload_msg.data()));
                        break;
                    }
                    case krypto::utils::MsgType::TRADE: {
                        process(flatbuffers::GetRoot<krypto::serialization::Trade>(payload_msg.data()));
                        break;
                    }
                    case krypto::utils::MsgType::HEARTBEAT: {
                        process(flatbuffers::GetRoot<krypto::serialization::Heartbeat>(payload_msg.data()));
                        break;
                    }
                    case krypto::utils::MsgType::ORDER_UPDATE: {
                        // Ignore
                        break;
                    }
                    default: {
                        KRYP_LOG(info, "Ignoring unknown message type = {}", topic);
                    }
                }
            }

            if (items[1].revents && ZMQ_POLLIN) {
                krypto::network::recv_empty_frame(*receiver_);
                auto address = krypto::network::recv_string(*receiver_);
                auto msg_type = krypto::network::recv_msg_type(*receiver_);
                zmq::message_t payload_msg;
                const auto payload_size = receiver_->recv(payload_msg, zmq::recv_flags::none);

                if (!payload_size.has_value()) {
                    KRYP_LOG(error, "Payload has 0 size");
                }

                KRYP_LOG(info, "Received request with payload size {} from {}",
                         payload_msg.size(), address);

                switch (msg_type) {
                    case krypto::utils::MsgType::ORDER_REQUEST: {
                        process(flatbuffers::GetRoot<krypto::serialization::OrderRequest>(payload_msg.data()), address);
                        break;
                    }
                    case krypto::utils::MsgType::ORDER_CANCEL_REQUEST: {
                        process(flatbuffers::GetRoot<krypto::serialization::OrderCancelRequest>(payload_msg.data()),
                                address);
                        break;
                    }
                    case krypto::utils::MsgType::ORDER_REPLACE_REQUEST: {
                        process(flatbuffers::GetRoot<krypto::serialization::OrderReplaceRequest>(payload_msg.data()),
                                address);
                        break;
                    }
                    default: {
                        KRYP_LOG(info, "Ignoring unknown message type. ");
                    }
                }

            }
        }

        send_disconnect(identity);
    }

    template<krypto::serialization::Exchange Exchange>
    void OrderServer<Exchange>::stop() {
        running_ = false;
    }

    template<krypto::serialization::Exchange Exchange>
    void OrderServer<Exchange>::send_reject(
            const krypto::serialization::OrderStatus order_status,
            const std::string &order_id,
            const std::string &client_identity) {
        const krypto::utils::OrderUpdate rejected{
                krypto::utils::current_time_in_nanoseconds(),
                order_id,
                order_id,
                order_status,
                0
        };
        send(rejected, client_identity);
    }

    template<krypto::serialization::Exchange Exchange>
    void OrderServer<Exchange>::send(const krypto::utils::OrderUpdate &order_update,
                                              const std::string &client_identity) {
        KRYP_LOG(info, "{} :: {}",
                 krypto::serialization::EnumNameOrderStatus(
                         order_update.status), order_update.order_id);

        krypto::network::send_empty_frame(*receiver_, zmq::send_flags::sndmore);
        krypto::network::send_status(*receiver_, krypto::network::SocketStatus::REPLY, zmq::send_flags::sndmore);
        krypto::network::send_string(*receiver_, client_identity, zmq::send_flags::sndmore);
        krypto::network::send_msg_type(*receiver_, krypto::utils::MsgType::ORDER_UPDATE, zmq::send_flags::sndmore);
        serialize(order_update);
        krypto::network::send_fb_buffer(*receiver_, fb_builder_);
    }

    template<krypto::serialization::Exchange Exchange>
    void OrderServer<Exchange>::send_ready(const std::string &identity) {
        KRYP_LOG(info, "Sending ready status");
        krypto::network::send_empty_frame(*receiver_, zmq::send_flags::sndmore);
        krypto::network::send_status(*receiver_, krypto::network::SocketStatus::READY);
    }

    template<krypto::serialization::Exchange Exchange>
    void OrderServer<Exchange>::send_disconnect(const std::string &identity) {
        KRYP_LOG(info, "Sending disconnect status");
        krypto::network::send_empty_frame(*receiver_, zmq::send_flags::sndmore);
        krypto::network::send_status(*receiver_, krypto::network::SocketStatus::DISCONNECT);
    }
}