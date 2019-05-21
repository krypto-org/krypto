#pragma once

#include <vector>

#include <zmq.hpp>


namespace krypto::network {
    class ZmqProxy final {
    private:
        const zmq::context_t &context_;
        std::string frontend_address_;
        std::string backend_address_;
        std::vector<std::string> topics_;
        zmq::socket_t frontend_;
        zmq::socket_t backend_;
    public:
        ZmqProxy(zmq::context_t &, std::string, std::string,
                 std::initializer_list<std::string>);

        ~ZmqProxy();

        ZmqProxy(const ZmqProxy &other) = delete;

        ZmqProxy(ZmqProxy &&other) = delete;

        ZmqProxy &operator=(const ZmqProxy &other) = delete;

        ZmqProxy &operator=(ZmqProxy &&other) = delete;

        void start();
    };
}

