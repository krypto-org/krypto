#pragma once

#include <vector>

#include <zmq.hpp>

#include <krypto/config.h>

namespace krypto::mktdata {
    // TODO: Convert to last value cache based gateway

    class MktdataGateway final {
    private:
        std::string frontend_address_;
        std::string backend_address_;
        std::unique_ptr<zmq::socket_t> frontend_;
        std::unique_ptr<zmq::socket_t> backend_;
        std::atomic_bool running_;
    public:
        MktdataGateway(zmq::context_t &context, const krypto::Config &config);

        void start();

        void stop();
    };
}
