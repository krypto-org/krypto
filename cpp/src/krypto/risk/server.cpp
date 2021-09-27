#include <krypto/risk/server.h>
#include <krypto/network/helper.h>
#include <krypto/instruments/client.h>

namespace krypto::risk {

    Server::Server(zmq::context_t &context, const krypto::Config &config)
            : position_manager_{krypto::instruments::InstrumentClient(context, config),
                                config.at<std::string>("/external/redis")},
              responder_{std::make_unique<zmq::socket_t>(context, ZMQ_ROUTER)},
              order_subscriber_{std::make_unique<zmq::socket_t>(context, ZMQ_SUB)},
              responder_endpoint_{config.at<std::string>("/services/risk/server")},
              order_endpoint_{config.at<std::string>("/services/order_gateway/broadcast/client")},
              running_{false} {
    }

    void Server::start() {
        KRYP_LOG(info, "Starting risk server @ {}", responder_endpoint_);
        try {
            responder_->bind(responder_endpoint_.c_str());
        }
        catch (zmq::error_t &e) {
            KRYP_LOG(error, "Binding failed with error {}", e.what());
            return;
        }

        try {
            order_subscriber_->connect(order_endpoint_.c_str());
        }
        catch (zmq::error_t &e) {
            KRYP_LOG(error, "Binding failed with error {}", e.what());
            return;
        }

        running_ = true;

        order_subscriber_->set(zmq::sockopt::subscribe, "");

        zmq::pollitem_t items[] = {
                {*responder_,        0, ZMQ_POLLIN, 0},
                {*order_subscriber_, 0, ZMQ_POLLIN, 0},
        };

        while (running_) {
            zmq::poll(&items[0], 2, 10);

            if (items[0].revents && ZMQ_POLLIN) {
                auto address = krypto::network::recv_string(*responder_);
                krypto::network::recv_empty_frame(*responder_);
                auto msg_type = krypto::network::recv_msg_type(*responder_);
                zmq::message_t payload_msg;
                auto received = responder_->recv(payload_msg);

                if (!received.has_value()) {
                    KRYP_LOG(error, "No message received from client");
                    continue;
                }

                KRYP_LOG(info, "Received request with payload size {} from {}",
                         payload_msg.size(), address);

                bool send_reply = process(payload_msg, msg_type);
                if (send_reply) {
                    krypto::network::send_string(*responder_, address, zmq::send_flags::sndmore);
                    krypto::network::send_empty_frame(*responder_, zmq::send_flags::sndmore);
                    zmq::message_t result_msg(fb_builder_.GetSize());
                    std::memcpy(result_msg.data(),
                                fb_builder_.GetBufferPointer(), fb_builder_.GetSize());
                    responder_->send(result_msg, zmq::send_flags::none);
                    KRYP_LOG(info, "Sent result to {}", address);
                }
            }

            if (items[1].revents && ZMQ_POLLIN) {
                auto client = krypto::network::recv_string(*order_subscriber_);
                auto msg_type = krypto::network::recv_msg_type(*order_subscriber_);
                zmq::message_t payload;
                auto received = responder_->recv(payload);

                if (!received.has_value()) {
                    KRYP_LOG(error, "No message received from order broadcaster");
                    continue;
                }

                if (msg_type == krypto::utils::MsgType::ORDER_UPDATE) {
                    auto order_update = flatbuffers::GetRoot<
                            krypto::serialization::OrderUpdate>(payload.data());
                    auto fill = order_manager_.process_update(order_update);
                    if (fill.has_value()) {
                        position_manager_.add_fill(fill.value());
                    }
                } else if (msg_type == krypto::utils::MsgType::ORDER_REQUEST) {
                    auto order_request = flatbuffers::GetRoot<
                            krypto::serialization::OrderRequest>(payload.data());
                    order_manager_.register_order(order_request);
                }
            }

        }
    }

    void Server::stop() {
        running_ = false;
    }

    bool Server::process(const zmq::message_t &msg, const krypto::utils::MsgType &msg_type) {
        if (msg_type == utils::MsgType::RISK_SUMMARY_REQUEST) {
            position_manager_.update_pnl();
            // TODO: Process Position Request
        }
        return fb_builder_.GetSize() > 0;
    }
}
