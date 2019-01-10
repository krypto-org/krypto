#pragma once

#include <atomic>

#include <krypto/mktdata/book.h>
#include <tbb/concurrent_queue.h>
#include <nlohmann/json.hpp>

namespace krypto::mktdata::coinbase {
    class BookBuilder final {
    private:
        static constexpr int QUEUE_SIZE = 4096;

        std::atomic_bool snapshot_received_;

        tbb::concurrent_bounded_queue<nlohmann::json> incr_queue_;
        OrderBook book_;

        void apply_incremental(
                double_t price,
                double_t qty,
                OrderSide side);

    public:
        explicit BookBuilder();

        ~BookBuilder();

        BookBuilder(const BookBuilder &other) = delete;

        BookBuilder(BookBuilder &&other) = delete;

        BookBuilder &operator=(const BookBuilder &other) = delete;

        BookBuilder &operator=(BookBuilder &&other) = delete;

        void handle_incr(nlohmann::json incremental);

        void handle_snap(nlohmann::json snapshot);

        void handle_trade(nlohmann::json trade);

        const OrderBook& book() const;
    };
}
