#pragma once

#include <vector>

#include <zmq.hpp>

#include <krypto/config.h>

namespace krypto::network {
    class ZmqProxy final {
    private:
        zmq::context_t context_;
        std::string frontend_address_;
        std::string backend_address_;
        std::unique_ptr<zmq::socket_t> frontend_;
        std::unique_ptr<zmq::socket_t> backend_;
        std::atomic_bool running_;
    public:
        explicit ZmqProxy(const krypto::Config &config, const std::string& service_type);

        ~ZmqProxy();

        ZmqProxy(const ZmqProxy &other) = delete;

        ZmqProxy(ZmqProxy &&other) = delete;

        ZmqProxy &operator=(const ZmqProxy &other) = delete;

        ZmqProxy &operator=(ZmqProxy &&other) = delete;

        void start();

        void stop();
    };
}

