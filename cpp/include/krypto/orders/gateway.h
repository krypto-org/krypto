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
}
