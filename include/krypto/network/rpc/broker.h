#pragma once

#include <atomic>
#include <string>
#include <queue>
#include <unordered_map>

#include <zmq.hpp>

namespace krypto::network::rpc {
    class Broker final {
    private:
        const zmq::context_t& context_;
        std::string frontend_address_;
        std::string backend_address_;
        std::unique_ptr<zmq::socket_t> frontend_;
        std::unique_ptr<zmq::socket_t> backend_;
        std::atomic_bool running_;
        std::unordered_map<std::string, std::string> workers_;
        bool bind();
    public:
        Broker(zmq::context_t &, std::string, std::string);
        void start();
        void stop();
    };

}