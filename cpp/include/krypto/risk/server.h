#pragma once

#include <zmq.hpp>
#include <krypto/utils/types.h>
#include <krypto/orders/manager.h>
#include <krypto/config.h>
#include <krypto/risk/position_manager.h>

namespace krypto::risk {
    class Server final {
    private:
        PositionManager position_manager_;
        std::unique_ptr<zmq::socket_t> responder_;
        std::unique_ptr<zmq::socket_t> order_subscriber_;
        std::string responder_endpoint_;
        std::string order_endpoint_;
        std::atomic_bool running_{};
        flatbuffers::FlatBufferBuilder fb_builder_;
        krypto::orders::OrderManager order_manager_;

        bool process(const zmq::message_t &msg, const krypto::utils::MsgType &msg_type);

    public:
        Server(zmq::context_t &context, const krypto::Config &config);

        void start();

        void stop();
    };
}