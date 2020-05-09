#pragma once

#include <unordered_map>
#include <future>

#include <krypto/mktdata/coinbase/book_builder.h>
#include <krypto/config.h>
#include <krypto/mktdata/coinbase/ws.h>

namespace krypto::mktdata::coinbase {
    template<bool Verbose>
    class Server final {
    private:
        WsConnection::channel_t update_channel_{2};
        WsConnection ws_;
        BookBuilder<Verbose> book_builder_;
        bool publish_market_data_;
    public:
        Server(zmq::context_t &context,
               const krypto::Config &config,
               const std::string &environment,
               bool publish_md = true);

        void start();

        void stop();
    };

    template<bool Verbose>
    Server<Verbose>::Server(zmq::context_t &context,
                            const krypto::Config &config,
                            const std::string &environment,
                            const bool publish_md) :
            update_channel_{2},
            ws_{context, config, environment, update_channel_},
            book_builder_{context, config},
            publish_market_data_{publish_md} {
    }

    template<bool Verbose>
    void Server<Verbose>::start() {

        boost::fibers::fiber ws_fiber([this]() {
            ws_.start();
        });
        boost::fibers::fiber bb_fiber([this]() {
            nlohmann::json update;
            while (boost::fibers::channel_op_status::success == update_channel_.pop(update)) {
                auto type = update["type"].get<std::string>();
                if (type == "snapshot") {
                    if (publish_market_data_) {
                        book_builder_.handle_snap(std::move(update));
                    }
                } else if (type == "l2update") {
                    if (publish_market_data_) {
                        book_builder_.handle_incr(std::move(update));
                    }
                } else if (type == "heartbeat") {
                    book_builder_.handle_heartbeat(std::move(update));
                } else if (type == "received") {
                    book_builder_.handle_order_received(update);
                } else if (type == "open") {
                    book_builder_.handle_order_open(update);
                } else if (type == "done") {
                    book_builder_.handle_order_done(update);
                } else if (type == "match" || type == "last_match") {
                    if (publish_market_data_) {
                        book_builder_.handle_trade(update);
                    }
                    if constexpr  (Verbose) {
                        KRYP_LOG(debug, "Match Message: {}", update.dump());
                    }
                    book_builder_.handle_order_match(update);
                } else if (type == "change") {
                    book_builder_.handle_order_change(update);
                } else if (type == "activate") {
                    // Ignore stop orders for now
                } else {
                    KRYP_LOG(debug, update.dump());
                }
            }
        });

        bb_fiber.join();
        ws_fiber.join();
    }

    template<bool Verbose>
    void Server<Verbose>::stop() {
        update_channel_.close();
        ws_.stop();
    }
}