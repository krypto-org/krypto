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
    public:
        explicit Server(const krypto::Config &config);

        void start();

        void stop();
    };

    template<bool Verbose>
    Server<Verbose>::Server(const krypto::Config &config) :
            update_channel_{2},
            ws_{config, update_channel_},
            book_builder_{config} {

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
                    book_builder_.handle_snap(std::move(update));
                } else if (type == "l2update") {
                    book_builder_.handle_incr(std::move(update));
                } else if (type == "match") {
                    book_builder_.handle_trade(std::move(update));
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