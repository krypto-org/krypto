#pragma once

#include <krypto/instruments/client.h>
#include <krypto/orders/protocol.h>
#include <krypto/mktdata/protocol.h>
#include <krypto/network/subscriber.h>
#include <krypto/network/rpc/worker.h>
#include <krypto/network/publisher.h>
#include <krypto/exchanges/coinbase/api.h>
#include <krypto/utils/types.h>
#include <krypto/utils/date_time.h>
#include <krypto/utils/message_types.h>
#include <krypto/serialization/helper.h>
#include <krypto/mktdata/convert.h>

#include <utility>

namespace krypto::orders::coinbase {
    template<bool Verbose = false>
    class OrderServer final {
    private:
        std::unique_ptr<zmq::socket_t> receiver_;
        std::unique_ptr<zmq::socket_t> mktdata_subscriber_;
        std::string order_gateway_endpoint_;
        std::string mktdata_gateway_endpoint_;
        krypto::exchanges::coinbase::Api api_;
        std::atomic_bool running_;
        bool is_sandbox_;
        std::unordered_map<std::string, krypto::utils::MsgType> msg_type_ref_;
        flatbuffers::FlatBufferBuilder fb_builder_;
        std::unordered_map<int64_t, krypto::utils::Instrument> instruments_;
        std::unordered_map<std::string, int64_t> exchange_symbol_to_id_map_;
        std::unordered_map<std::string, std::string> order_id_cl_order_id_map_;
        std::unordered_map<std::string, std::string> client_identity_map_;

        void serialize(const krypto::utils::OrderUpdate &);

        void send(const krypto::utils::OrderUpdate &order_update, const std::string &client_identity);

        void send_ready(const std::string &identity);

        void send_disconnect(const std::string &identity);

    public:
        OrderServer(zmq::context_t &,
                    krypto::exchanges::coinbase::Api api,
                    const krypto::Config &, bool is_sandbox);

        void process(const krypto::serialization::OrderRequest *, const std::string &client_identity);

        void process(const krypto::serialization::OrderCancelRequest *, const std::string &client_identity);

        void process(const krypto::serialization::OrderReplaceRequest *, const std::string &client_identity);

        void process(const krypto::serialization::Heartbeat *hb);

        void process(const krypto::serialization::OrderUpdate *order_update);

        void start();

        void stop();
    };


    template<bool Verbose>
    OrderServer<Verbose>::OrderServer(zmq::context_t &context,
                                      krypto::exchanges::coinbase::Api api,
                                      const Config &config, const bool is_sandbox):
            receiver_{std::make_unique<zmq::socket_t>(context, ZMQ_DEALER)},
            mktdata_subscriber_{std::make_unique<zmq::socket_t>(context, ZMQ_SUB)},
            order_gateway_endpoint_{config.at<std::string>(
                    "/services/order_gateway/backend/client")},
            mktdata_gateway_endpoint_{config.at<std::string>(
                    "/services/mktdata_gateway/backend/client")},
            api_{std::move(api)},
            running_{false},
            is_sandbox_{is_sandbox},
            msg_type_ref_{krypto::utils::name_to_msg_type()} {
        krypto::instruments::InstrumentClient client{context, config};
        auto query = client.query().get();
        for (auto &&inst: query) {
            instruments_[inst.id] = inst;
        }
        for (auto &&pair: instruments_) {
            exchange_symbol_to_id_map_[pair.second.exchange_symbol] = pair.first;
        }
    }

    template<bool Verbose>
    void OrderServer<Verbose>::serialize(const krypto::utils::OrderUpdate &order_update) {
        krypto::serialization::serialize(fb_builder_, order_update);
    }

    template<bool Verbose>
    void
    OrderServer<Verbose>::process(const krypto::serialization::OrderRequest *request,
                                  const std::string &client_identity) {
        krypto::exchanges::coinbase::Order order{
                instruments_[request->security_id()].exchange_symbol,
                request->side() == serialization::Side_BUY ? "buy" : "sell",
                krypto::mktdata::extract_price(request->price()),
                krypto::mktdata::extract_quantity(request->quantity()),
                "limit", request->order_id()->str()
        };

        KRYP_LOG(info, "Received a new order request from {} ", order);

        if (request->tif() != krypto::serialization::TimeInForce_DAY) {
            order.tif = krypto::serialization::EnumNameTimeInForce(request->tif());
        }
        auto order_ack = api_.place_order(order);
        if (order_ack.has_value()) {
            KRYP_LOG(info, "Order ack from coinbase: {}", order_ack.value().dump());
            auto order_id = order_ack.value().at("id").get<std::string>();
            const krypto::utils::OrderUpdate in_flight{
                    krypto::utils::current_time_in_nanoseconds(),
                    request->order_id()->str(),
                    order_id,
                    krypto::serialization::OrderStatus::OrderStatus_IN_FLIGHT,
                    0};
            send(in_flight, client_identity);
            order_id_cl_order_id_map_[order_id] = request->order_id()->str();
            client_identity_map_[order_id] = client_identity;
        } else {
            KRYP_LOG(info, "No ack received for clordid: {}", request->order_id()->str());
            const krypto::utils::OrderUpdate reject{
                    krypto::utils::current_time_in_nanoseconds(),
                    request->order_id()->str(),
                    "",
                    krypto::serialization::OrderStatus::OrderStatus_REJECTED,
                    0};
            send(reject, client_identity);
        }
    }

    template<bool Verbose>
    void
    OrderServer<Verbose>::process(const krypto::serialization::OrderCancelRequest * request,
                                  const std::string &client_identity) {
        KRYP_LOG(info, "Cancel order: {}", request->order_id()->str());

        auto status = api_.cancel_order(request->order_id()->str());
        if (status.has_value()) {
            const krypto::utils::OrderUpdate in_flight{
                    krypto::utils::current_time_in_nanoseconds(),
                    order_id_cl_order_id_map_[request->order_id()->str()],
                    request->order_id()->str(),
                    krypto::serialization::OrderStatus::OrderStatus_CANCEL_IN_FLIGHT,
                    0};
            send(in_flight, client_identity);
        }
    }

    template<bool Verbose>
    void
    OrderServer<Verbose>::process(const krypto::serialization::OrderReplaceRequest * request,
                                  const std::string &client_identity) {
        KRYP_LOG(warn, "Replace not supported. Sending reject");
        const krypto::utils::OrderUpdate reject{
                krypto::utils::current_time_in_nanoseconds(),
                request->order_id()->str(),
                "",
                krypto::serialization::OrderStatus::OrderStatus_REPLACE_REJECTED,
                0};
        send(reject, client_identity);
    }

    template<bool Verbose>
    void OrderServer<Verbose>::start() {
        std::string identity = krypto::serialization::EnumNameExchange(krypto::serialization::Exchange_COINBASE);
        if (is_sandbox_) {
            identity += "_SANDBOX";
        }
        krypto::network::connect(*receiver_, order_gateway_endpoint_, identity);
        KRYP_LOG(info, "Connecting to market data gateway @ {}", mktdata_gateway_endpoint_);
        mktdata_subscriber_->connect(mktdata_gateway_endpoint_);

        if constexpr (Verbose) {
            KRYP_LOG(info, "Connected to gateway backend @ {}", order_gateway_endpoint_);
        }

        send_ready(identity);

        if constexpr (Verbose) {
            KRYP_LOG(info, "Sent ready status to broker @ {}", order_gateway_endpoint_);
        }

        zmq::pollitem_t items[] = {
                {*mktdata_subscriber_, 0, ZMQ_POLLIN, 0},
                {*receiver_,           0, ZMQ_POLLIN, 0}
        };

        running_ = true;

        const std::string subscription = krypto::utils::MsgTypeNames[
                static_cast<uint8_t>(krypto::utils::MsgType::ORDER_UPDATE)];
        mktdata_subscriber_->set(zmq::sockopt::subscribe, subscription);

        while (running_) {
            zmq::poll(&items[0], 2, 0);
            if (items[0].revents && ZMQ_POLLIN) {

                auto topic = krypto::network::recv_string(*mktdata_subscriber_);
                auto topic_prefix = topic.substr(0, 2);
                auto msg_type = msg_type_ref_.at(topic_prefix);

                zmq::message_t payload_msg;
                const auto payload_size = mktdata_subscriber_->recv(payload_msg);

                if (!payload_size.has_value()) {
                    KRYP_LOG(error, "Payload has 0 size");
                    break;
                }

                switch (msg_type) {
                    case krypto::utils::MsgType::HEARTBEAT: {
                        process(flatbuffers::GetRoot<krypto::serialization::Heartbeat>(payload_msg.data()));
                        break;
                    }
                    case krypto::utils::MsgType::ORDER_UPDATE: {
                        process(flatbuffers::GetRoot<krypto::serialization::OrderUpdate>(payload_msg.data()));
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
                const auto payload_size = receiver_->recv(payload_msg);

                if (!payload_size.has_value()) {
                    KRYP_LOG(error, "Payload has an empty frame. No message received.");
                    break;
                }

                if constexpr (Verbose) {
                    KRYP_LOG(info, "Received request with payload size {} from {}",
                             payload_msg.size(), address);
                }

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

    template<bool Verbose>
    void OrderServer<Verbose>::stop() {
        running_ = false;
    }

    template<bool Verbose>
    void OrderServer<Verbose>::send(const utils::OrderUpdate &order_update, const std::string &client_identity) {
        if constexpr (Verbose) {
            KRYP_LOG(info, "{} :: {} || {}",
                     krypto::serialization::EnumNameOrderStatus(
                             order_update.status),
                             order_update.order_id,
                             order_update.exchange_order_id);
        }
        krypto::network::send_empty_frame(*receiver_, zmq::send_flags::sndmore);
        krypto::network::send_status(*receiver_, krypto::network::SocketStatus::REPLY, zmq::send_flags::sndmore);
        krypto::network::send_string(*receiver_, client_identity, zmq::send_flags::sndmore);
        krypto::network::send_msg_type(*receiver_, krypto::utils::MsgType::ORDER_UPDATE, zmq::send_flags::sndmore);
        serialize(order_update);
        krypto::network::send_fb_buffer(*receiver_, fb_builder_);
    }

    template<bool Verbose>
    void OrderServer<Verbose>::send_ready(const std::string &identity) {
        if constexpr (Verbose) {
            KRYP_LOG(info, "Sending ready status");
        }

        krypto::network::send_empty_frame(*receiver_, zmq::send_flags::sndmore);
        krypto::network::send_status(*receiver_, krypto::network::SocketStatus::READY);
    }

    template<bool Verbose>
    void OrderServer<Verbose>::send_disconnect(const std::string &identity) {
        if constexpr (Verbose) {
            KRYP_LOG(info, "Sending disconnect status");
        }

        krypto::network::send_empty_frame(*receiver_, zmq::send_flags::sndmore);
        krypto::network::send_status(*receiver_, krypto::network::SocketStatus::DISCONNECT);
    }


    template<bool Verbose>
    void OrderServer<Verbose>::process(const krypto::serialization::Heartbeat *hb) {
        KRYP_LOG(info, "Heartbeat from coinbase mktdata server for  {}", hb->security_id());
    }

    template<bool Verbose>
    void OrderServer<Verbose>::process(const krypto::serialization::OrderUpdate *order_update) {
        auto exchange_order_id = order_update->exchange_order_id()->str();
        if (order_id_cl_order_id_map_.find(exchange_order_id) !=
            std::end(order_id_cl_order_id_map_)) {
            KRYP_LOG(info, "Order update {}", order_update->exchange_order_id()->str());

            KRYP_LOG(debug, "Order found for {}", exchange_order_id);
            const krypto::utils::OrderUpdate update{
                    krypto::utils::current_time_in_nanoseconds(),
                    order_id_cl_order_id_map_[exchange_order_id],
                    exchange_order_id,
                    order_update->status(),
                    order_update->filled_quantity()};
            auto client_identity = client_identity_map_[exchange_order_id];
            send(update, client_identity);
            if (order_update->status() == krypto::serialization::OrderStatus_DONE ||
                order_update->status() == krypto::serialization::OrderStatus_CANCELLED ||
                order_update->status() == krypto::serialization::OrderStatus_REPLACED) {
                KRYP_LOG(info, "Order id - {} - done --- removing from cache", exchange_order_id);
                order_id_cl_order_id_map_.erase(exchange_order_id);
                client_identity_map_.erase(exchange_order_id);
            }
        }
    }
}
