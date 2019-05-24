#include <iostream>
#include <string>

#include <krypto/logger.h>
#include <krypto/mktdata/coinbase/ws.h>
#include <krypto/mktdata/coinbase/book_builder.h>

namespace coinbase = krypto::mktdata::coinbase;

namespace {
    volatile std::sig_atomic_t SIGNAL_STATUS;
}

std::function<void(int)> shutdown_handler;

void signal_handler(int signal) { shutdown_handler(signal); }

int main() {

    zmq::context_t context(1);

    krypto::network::mktdata::TopOfBookPublisher publisher{context, "tcp://127.0.0.1:23400"};
    publisher.start<false>();

    coinbase::BookBuilder<krypto::network::mktdata::TopOfBookPublisher> bb{publisher};

    coinbase::WsConnection ws{"wss://ws-feed.pro.coinbase.com", [&bb](nlohmann::json &&msg) {
        auto type = msg["type"].get<std::string>();
        if (type == "snapshot") {
            bb.handle_snap(std::move(msg));
        } else if (type == "l2update") {
            bb.handle_incr(std::move(msg));
        } else if (type == "match") {
            bb.handle_trade(std::move(msg));
        } else {
            KRYP_LOG(debug, msg.dump());
        }
    }, R"({ "type": "subscribe", "product_ids": ["BTC-USD"], "channels": ["level2", "heartbeat", "matches"]})"};

    std::thread ws_processing_thread(std::bind(&coinbase::WsConnection::start, &ws));

    shutdown_handler = [&ws](int signal) {
        SIGNAL_STATUS = signal;
        ws.stop();
    };
    std::signal(SIGINT, signal_handler);

    ws_processing_thread.join();
    using namespace std::chrono_literals;

    std::this_thread::sleep_for(2s);

    KRYP_LOG(info, "Closed Client Connection. Exiting");
    return 0;
}