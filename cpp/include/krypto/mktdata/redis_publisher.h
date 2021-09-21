#pragma once

#include <krypto/network/subscriber.h>
#include <krypto/config.h>
#include <sw/redis++/redis++.h>


namespace krypto::mktdata {
    class RedisPublisher final {
    private:
        std::unique_ptr<zmq::socket_t> mktdata_subscriber_;
        std::string mktdata_gateway_endpoint_;
        sw::redis::Redis redis_;
        std::atomic_bool running_;
    public:
        RedisPublisher(zmq::context_t &context, const krypto::Config &config);

        void process(const krypto::serialization::Quote *quote);

        void start();

        void stop();
    };
}