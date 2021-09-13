#include <krypto/mktdata/coinbase/server.h>

namespace krypto::mktdata::coinbase {

    Server::Server(zmq::context_t &context,
                   const krypto::Config &config,
                   const std::string &environment,
                   const bool publish_md) :
            update_channel_{2},
            ws_{context, config, environment, update_channel_},
            book_builder_{context, config},
            publish_market_data_{publish_md} {
    }

    void Server::start() {

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

    void Server::stop() {
        update_channel_.close();
        ws_.stop();
    }

}