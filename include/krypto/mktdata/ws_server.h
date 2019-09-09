#pragma once

#include <iostream>
#include <set>
#include <cstdint>
#include <utility>
#include <krypto/config.h>

#include <tbb/concurrent_queue.h>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <krypto/mktdata/book.h>
#include <krypto/network/subscriber.h>
#include <krypto/serialization/serialization_generated.h>

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

using websocketpp::lib::thread;
using websocketpp::lib::mutex;
using websocketpp::lib::lock_guard;
using websocketpp::lib::unique_lock;
using websocketpp::lib::condition_variable;



namespace krypto::mktdata {
    typedef websocketpp::server<websocketpp::config::asio> server_t;

    class WebsocketServer final : public krypto::network::Subscriber<WebsocketServer, false> {
    public:
        explicit WebsocketServer(const krypto::Config& config);
        void ioc_run();
        void done();
        void on_open(const connection_hdl& hdl);
        void on_close(const connection_hdl& hdl);
        void on_message(const connection_hdl& hdl, const server_t::message_ptr& msg);
        void process_messages();
        // Mktdata Subscriber Interface
        void process(const krypto::serialization::Quote *quote);
        void process(const krypto::serialization::Trade *trade);
        void process(const krypto::serialization::Heartbeat *hb);
    private:
        typedef std::set<connection_hdl,std::owner_less<connection_hdl> > con_list_t;

        uint16_t port_;
        server_t server_;
        con_list_t connections_;
        tbb::concurrent_bounded_queue<std::string> message_queue_;
        std::unordered_map<uint64_t, Quote> quotes_;
        std::mutex connection_lock_;
        std::atomic_bool running_;
        uint64_t next_update_ts_;
    };
}