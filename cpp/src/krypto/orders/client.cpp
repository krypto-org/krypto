#include <krypto/orders/client.h>

namespace krypto::orders {
    OrderClient::OrderClient(zmq::context_t &context, const Config &config) :
            sender_{std::make_unique<zmq::socket_t>(context, ZMQ_DEALER)},
            queue_push_{std::make_unique<zmq::socket_t>(context, ZMQ_PUSH)},
            queue_pull_{std::make_unique<zmq::socket_t>(context, ZMQ_PULL)},
            gateway_endpoint_{config.at<std::string>(
                    "/services/order_gateway/frontend/client")},
            running_{false} {
        KRYP_LOG(info, "Connecting to gateway @ {}", gateway_endpoint_);
        queue_push_->connect(queue_endpoint);
    }

    std::thread OrderClient::start() {
        return std::thread([this]() {
            poll_sockets();
        });
    }

    void OrderClient::poll_sockets() {
        auto identity = "orders-" + krypto::network::generate_id();
        krypto::network::connect(*sender_, gateway_endpoint_, identity);
        queue_pull_->bind(queue_endpoint);

        zmq::pollitem_t items[] = {
                {*queue_pull_, 0, ZMQ_POLLIN, 0},
                {*sender_,     0, ZMQ_POLLIN, 0}
        };

        running_ = true;

        while (running_) {
            zmq::poll(&items[0], 2, 0);

            if (items[0].revents && ZMQ_POLLIN) {
                /*
                 * IN: EXCHANGE -> MSG_TYPE -> PAYLOAD
                 * */
                auto exchange = krypto::network::recv_string(*queue_pull_);
                auto msg_type = krypto::network::recv_msg_type(*queue_pull_);
                zmq::message_t payload;
                const auto payload_size = queue_pull_->recv(payload, zmq::recv_flags::none);

                if (!payload_size.has_value()) {
                    KRYP_LOG(error, "Payload has 0 size");
                    break;
                }

                /*
                 * OUT: IDENTITY -> EMPTY -> EXCHANGE -> MSG_TYPE -> PAYLOAD
                 * */
                krypto::network::send_empty_frame(*sender_, zmq::send_flags::sndmore);
                krypto::network::send_string(*sender_, exchange, zmq::send_flags::sndmore);
                krypto::network::send_msg_type(*sender_, msg_type, zmq::send_flags::sndmore);
                sender_->send(payload, zmq::send_flags::none);
            }

            if (items[1].revents && ZMQ_POLLIN) {
                /*
                 * IN: EMPTY -> EXCHANGE -> MSG_TYPE -> PAYLOAD
                 * */
                krypto::network::recv_empty_frame(*sender_);
                auto exchange = krypto::network::recv_string(*sender_);
                auto msg_type = krypto::network::recv_msg_type(*sender_);

                if (msg_type == krypto::utils::MsgType::UNDEFINED) {
                    KRYP_LOG(error, "Received undefined message");
                    continue;
                }

                if (msg_type == krypto::utils::MsgType::NO_PAYLOAD) {
                    KRYP_LOG(info, "Received acknowledgement message - no payload");
                    continue;
                }

                zmq::message_t payload_msg;
                const auto payload_size = sender_->recv(payload_msg);


                if (!payload_size.has_value()) {
                    KRYP_LOG(error, "Payload has no frame. No message received.");
                    return;
                }

                if (msg_type == krypto::utils::MsgType::ORDER_UPDATE) {
                    auto order_update = flatbuffers::GetRoot<
                            krypto::serialization::OrderUpdate>(
                            payload_msg.data());
                    process(order_update);
                }
            }
        }
    }

    void OrderClient::stop() {
        running_ = false;
    }

    void OrderClient::serialize(const krypto::utils::OrderRequest &request) {
        fb_builder_.Clear();
        auto oid_offset = fb_builder_.CreateString(request.order_id);
        krypto::serialization::OrderRequestBuilder builder{fb_builder_};
        builder.add_order_id(oid_offset);
        builder.add_timestamp(request.timestamp);
        builder.add_security_id(request.security_id);
        builder.add_price(request.price);
        builder.add_quantity(request.quantity);
        builder.add_side(request.side);
        builder.add_tif(request.tif);
        auto req = builder.Finish();
        fb_builder_.Finish(req);
    }

    void OrderClient::serialize(const krypto::utils::OrderCancelRequest &request) {
        fb_builder_.Clear();
        auto oid_offset = fb_builder_.CreateString(request.order_id);
        krypto::serialization::OrderCancelRequestBuilder builder{fb_builder_};
        builder.add_order_id(oid_offset);
        builder.add_timestamp(request.timestamp);
        auto req = builder.Finish();
        fb_builder_.Finish(req);
    }

    void OrderClient::serialize(const krypto::utils::OrderReplaceRequest &request) {
        fb_builder_.Clear();
        auto oid_offset = fb_builder_.CreateString(request.order_id);
        krypto::serialization::OrderReplaceRequestBuilder builder{fb_builder_};
        builder.add_order_id(oid_offset);
        builder.add_timestamp(request.timestamp);
        builder.add_price(request.price);
        builder.add_quantity(request.quantity);
        builder.add_side(request.side);
        auto req = builder.Finish();
        fb_builder_.Finish(req);
    }

    void OrderClient::process(const krypto::serialization::OrderUpdate *order_update) {
        std::for_each(std::begin(callbacks_), std::end(callbacks_), [&](auto &&c) {
            c(order_update);
        });
    }

    std::string
    OrderClient::new_order(
            const std::string &exchange,
            int64_t security_id,
            int64_t price,
            int64_t size,
            krypto::serialization::Side side,
            krypto::serialization::TimeInForce tif) {

        auto oid = order_id_generator_.new_id();
        krypto::utils::OrderRequest order_request{
                krypto::utils::current_time_in_nanoseconds(),
                security_id,
                price,
                size,
                side,
                oid,
                tif};

        serialize(order_request);
        krypto::network::send_string(
                *queue_push_, exchange, zmq::send_flags::sndmore);
        krypto::network::send_msg_type(
                *queue_push_,
                krypto::utils::MsgType::ORDER_REQUEST,
                zmq::send_flags::sndmore);
        krypto::network::send_fb_buffer(*queue_push_, fb_builder_);
        return oid;
    }

    void OrderClient::cancel_order(
            const std::string &exchange, const std::string &order_id) {
        krypto::utils::OrderCancelRequest cancel{
                krypto::utils::current_time_in_nanoseconds(),
                order_id};
        serialize(cancel);
        krypto::network::send_string(
                *queue_push_, exchange, zmq::send_flags::sndmore);
        krypto::network::send_msg_type(
                *queue_push_,
                krypto::utils::MsgType::ORDER_CANCEL_REQUEST,
                zmq::send_flags::sndmore);
        KRYP_LOG(info, "Queue-ing cancel for {} to send", order_id);
        krypto::network::send_fb_buffer(*queue_push_, fb_builder_);
    }

    void OrderClient::replace_order(
            const std::string &exchange,
            const std::string &order_id,
            int64_t price, int64_t size,
            krypto::serialization::Side side) {
        krypto::utils::OrderReplaceRequest replace_request{
                krypto::utils::current_time_in_nanoseconds(),
                order_id,
                price,
                size,
                side};
        serialize(replace_request);
        krypto::network::send_string(
                *queue_push_, exchange, zmq::send_flags::sndmore);
        krypto::network::send_msg_type(
                *queue_push_,
                krypto::utils::MsgType::ORDER_REPLACE_REQUEST,
                zmq::send_flags::sndmore);
        KRYP_LOG(info, "Queue-ing replace for {} to send", order_id);
        krypto::network::send_fb_buffer(*queue_push_, fb_builder_);
    }

    void OrderClient::register_listener(const OrderClient::order_update_func_t &listener) {
        callbacks_.push_back(listener);
    }

    void OrderClient::unregister_listener(const OrderClient::order_update_func_t &listener) {

    }
}

