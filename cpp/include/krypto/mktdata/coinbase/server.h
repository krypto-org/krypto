#pragma once

#include <unordered_map>
#include <future>

#include <krypto/mktdata/coinbase/book_builder.h>
#include <krypto/config.h>
#include <krypto/mktdata/coinbase/ws.h>

namespace krypto::mktdata::coinbase {
    class Server final {
    private:
        WsConnection::channel_t update_channel_{2};
        WsConnection ws_;
        BookBuilder book_builder_;
        bool publish_market_data_;
    public:
        Server(zmq::context_t &context,
               const krypto::Config &config,
               const std::string &environment,
               bool publish_md = true);

        void start();

        void stop();
    };

}