#pragma once


#include <atomic>
#include <string>
#include <queue>
#include <unordered_map>

#include <zmq.hpp>

#include <krypto/config.h>
#include <krypto/logger.h>
#include <krypto/network/helper.h>
#include <krypto/utils/types.h>

namespace krypto::orders {

    template<bool Verbose = false>
    class OrderGateway final {
    private:
        std::string frontend_address_;
        std::string backend_address_;
        std::unique_ptr<zmq::socket_t> frontend_;
        std::unique_ptr<zmq::socket_t> backend_;
        std::atomic_bool running_;
        std::unordered_map<std::string, std::string> workers_;

        bool bind();

    public:
        OrderGateway(zmq::context_t &, const krypto::Config &);

        void start();

        void stop();
    };


    template<bool Verbose>
    OrderGateway<Verbose>::OrderGateway(zmq::context_t &context, const krypto::Config &config) :
            frontend_address_{config.at<std::string>("/services/order_gateway/frontend/server")},
            backend_address_{config.at<std::string>("/services/order_gateway/backend/server")},
            frontend_{std::make_unique<zmq::socket_t>(context, ZMQ_ROUTER)},
            backend_{std::make_unique<zmq::socket_t>(context, ZMQ_ROUTER)},
            running_{false} {}

    template<bool Verbose>
    bool OrderGateway<Verbose>::bind() {
        bool bound = false;
        KRYP_LOG(info, "Binding frontend to {}", frontend_address_);
        try {
            frontend_->bind(frontend_address_);
            bound = true;
        }
        catch (zmq::error_t &e) {
            KRYP_LOG(error, "Binding failed with {}", e.what());
        }
        KRYP_LOG(info, "Binding backend to {}", backend_address_);
        try {
            backend_->bind(backend_address_);
            bound = true;
        }
        catch (zmq::error_t &e) {
            KRYP_LOG(error, "Binding failed with {}", e.what());
        }
        if (!bound) {
            KRYP_LOG(warn, "Failed starting proxy - binding problems");
        }
        return bound;
    }

    template<bool Verbose>
    void OrderGateway<Verbose>::start() {
        running_ = bind();

        if (running_) {
            KRYP_LOG(info, "Started order gateway");
        }

        zmq::pollitem_t items[] = {
                {*backend_,  0, ZMQ_POLLIN, 0},
                {*frontend_, 0, ZMQ_POLLIN, 0}
        };

        while (running_) {

            zmq::poll(&items[0], 2, 10);

            if (items[0].revents && ZMQ_POLLIN) {
                /*
                 * IN (S): IDENTITY -> EMPTY -> STATUS -> [CLIENT -> MSG_TYPE -> PAYLOAD]
                 * OUT [C]: CLIENT_IDENTITY -> EMPTY -> EXCHANGE -> MSG_TYPE -> PAYLOAD
                 * */
                auto exchange = krypto::network::recv_string(*backend_);
                krypto::network::recv_empty_frame(*backend_);

                auto status = krypto::network::recv_status(*backend_);

                if (status == krypto::network::SocketStatus::READY) {
                    KRYP_LOG(info, "* <= {} :: ready @ address", exchange);
                    workers_[exchange] = exchange;
                } else if (status == krypto::network::SocketStatus::DISCONNECT) {
                    KRYP_LOG(info, "{} :: disconnected", exchange);
                    workers_.erase(exchange);
                } else if (status == krypto::network::SocketStatus::REPLY) {
                    auto client_addr = krypto::network::recv_string(*backend_);
                    auto msg_type = krypto::network::recv_msg_type(*backend_);
                    krypto::network::send_string(*frontend_, client_addr, zmq::send_flags::sndmore);
                    krypto::network::send_empty_frame(*frontend_, zmq::send_flags::sndmore);
                    krypto::network::send_string(*frontend_, exchange, zmq::send_flags::sndmore);

                    bool no_more_flag = msg_type == krypto::utils::MsgType::NO_PAYLOAD ||
                                        msg_type == krypto::utils::MsgType::UNDEFINED;
                    krypto::network::send_msg_type(*frontend_, msg_type, no_more_flag ? zmq::send_flags::none : zmq::send_flags::sndmore);
                    if (no_more_flag) {
                        continue;
                    }

                    zmq::message_t payload;
                    const auto payload_size = backend_->recv(payload, zmq::recv_flags::none);
                    if (!payload_size.has_value()) {
                        KRYP_LOG(error, "Payload had 0 size");
                        break;
                    }
                    if constexpr (Verbose) {
                        KRYP_LOG(info, "{} <= * <= {} :: {} + Payload Size: {}",
                                 client_addr, exchange,
                                 krypto::utils::MsgTypeNames[static_cast<int>(msg_type)], payload.size());
                    }
                    frontend_->send(payload, zmq::send_flags::none);
                }
            }

            if (items[1].revents && ZMQ_POLLIN) {
                /*
                 * IN (C): CLIENT_IDENTITY -> EMPTY -> EXCHANGE -> MSG_TYPE -> PAYLOAD
                 * OUT [S]: EXCHANGE -> EMPTY -> CLIENT_ADDRESS -> MSG_TYPE -> PAYLOAD
                 * */
                KRYP_LOG(info, "FRONTEND");
                auto client_addr = krypto::network::recv_string(*frontend_);
                krypto::network::recv_empty_frame(*frontend_);
                auto exchange = krypto::network::recv_string(*frontend_);
                auto msg_type = krypto::network::recv_msg_type(*frontend_);

                zmq::message_t request_payload;
                const auto payload_size = frontend_->recv(request_payload, zmq::recv_flags::none);

                if (!payload_size.has_value()) {
                    KRYP_LOG(error, "Payload has 0 size");
                    break;
                }

                if (workers_.find(exchange) != std::end(workers_)) {
                    auto worker_addr = workers_.at(exchange);
                    krypto::network::send_string(*backend_, worker_addr, zmq::send_flags::sndmore);
                    krypto::network::send_empty_frame(*backend_, zmq::send_flags::sndmore);
                    krypto::network::send_string(*backend_, client_addr, zmq::send_flags::sndmore);
                    if constexpr (Verbose) {
                        KRYP_LOG(info, "{} => * => {} @ {} : {} + Payload Size: {}",
                                 client_addr,
                                 exchange,
                                 worker_addr,
                                 krypto::utils::MsgTypeNames[static_cast<int>(msg_type)],
                                 request_payload.size());
                    }
                    krypto::network::send_msg_type(*backend_, msg_type, zmq::send_flags::sndmore);
                    backend_->send(request_payload, zmq::send_flags::none);
                } else {
                    KRYP_LOG(info, "Service not available");
                    krypto::network::send_string(*frontend_, client_addr, zmq::send_flags::sndmore);
                    krypto::network::send_empty_frame(*frontend_, zmq::send_flags::sndmore);
                    krypto::network::send_string(*frontend_, exchange, zmq::send_flags::sndmore);
                    krypto::network::send_msg_type(*frontend_, krypto::utils::MsgType::UNDEFINED);
                    if constexpr (Verbose) {
                        KRYP_LOG(info, "{} <= *",
                                 client_addr,
                                 krypto::utils::MsgTypeNames[static_cast<int>(msg_type)]);
                    }
                }
            }
        }
    }

    template<bool Verbose>
    void OrderGateway<Verbose>::stop() {
        running_ = false;
    }
}
