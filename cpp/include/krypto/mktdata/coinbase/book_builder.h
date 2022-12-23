#pragma once

#include <atomic>
#include <queue>

#include <tbb/concurrent_queue.h>
#include <nlohmann/json.hpp>

#include <krypto/mktdata/pub.h>
#include <krypto/mktdata/convert.h>
#include <krypto/instruments/client.h>
#include <krypto/utils/date_time.h>
#include <krypto/mktdata/protocol.h>
#include <krypto/utils/types.h>


namespace krypto::mktdata::coinbase {

    class BookBuilder final {
    private:
        std::unordered_map<std::string, int64_t> id_by_symbol_;
        std::unordered_map<std::string, std::unique_ptr<krypto::utils::OrderBook>> books;
        std::unordered_map<std::string, std::queue<nlohmann::json>> incr_queue_;
        std::unordered_map<std::string, bool> snapshot_received_;

        krypto::mktdata::Publisher publisher_;

        void apply_incremental(const std::string &symbol, int64_t price, int64_t qty, krypto::utils::OrderSide side);

        int64_t parse_time(const std::string &ts);

        void send_order_update(const krypto::utils::OrderUpdate &order_update);

    public:
        BookBuilder(zmq::context_t &context, const krypto::Config &config);

        ~BookBuilder();

        BookBuilder(const BookBuilder &other) = delete;

        BookBuilder(BookBuilder &&other) noexcept = delete;

        BookBuilder &operator=(const BookBuilder &other) = delete;

        BookBuilder &operator=(BookBuilder &&other) noexcept = delete;

        void handle_incr(nlohmann::json incremental);

        void handle_snap(nlohmann::json snapshot);

        void handle_trade(nlohmann::json trade);

        void handle_order_received(nlohmann::json received);

        void handle_order_open(nlohmann::json open_order);

        void handle_order_done(nlohmann::json done);

        void handle_order_match(nlohmann::json match);

        void handle_order_change(nlohmann::json change);

        void handle_heartbeat(nlohmann::json hb);
    };
}