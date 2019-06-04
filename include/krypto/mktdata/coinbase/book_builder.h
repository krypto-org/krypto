#pragma once

#include <atomic>

#include <krypto/mktdata/book.h>
#include <tbb/concurrent_queue.h>
#include <nlohmann/json.hpp>
#include <krypto/network/mktdata/top_of_book.h>
#include <krypto/mktdata/convert.h>

namespace krypto::mktdata::coinbase {
    template <typename Socket>
    class BookBuilder final {
    private:
        static constexpr int QUEUE_SIZE = 4096;

        Socket& socket_;
        std::atomic_bool snapshot_received_;
        tbb::concurrent_bounded_queue<nlohmann::json> incr_queue_;
        OrderBook book_;

        void apply_incremental(int64_t price, int64_t qty, OrderSide side);

    public:
        explicit BookBuilder(Socket& socket);

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

    template <typename Socket>
    BookBuilder<Socket>::BookBuilder(Socket &socket) :
            socket_(socket), snapshot_received_{false} {

        incr_queue_.set_capacity(QUEUE_SIZE);

        book_.quote.bid = 0;
        book_.quote.bid_qty = 0;

        book_.quote.ask = std::numeric_limits<int64_t >::max();
        book_.quote.ask_qty = 0;

        book_.quote.last = 0;
        book_.quote.last_qty = 0;
    }

    template <typename Socket>
    void BookBuilder<Socket>::handle_incr(nlohmann::json incremental) {
        if (snapshot_received_.load()) {

            auto incrs = incremental.at("changes").get<std::vector<std::vector<std::string>>>();

            for (auto &incr: incrs) {
                OrderSide side = incr[0] == "buy" ? OrderSide::BID : OrderSide::ASK;
                auto price = krypto::mktdata::convert_price(std::stod(incr[1]));
                auto qty = krypto::mktdata::convert_quantity(std::stod(incr[2]));

                apply_incremental(price, qty, side);
            }

            auto top_bid = std::begin(book_.bids);
            auto top_ask = std::begin(book_.asks);

            bool send = false;

            if (top_bid->first != book_.quote.bid) {
                book_.quote.bid = top_bid->first;
                send = true;
            }

            if (top_bid->second != book_.quote.bid_qty) {
                book_.quote.bid_qty = top_bid->second;
                send = true;
            }

            if (top_ask->first != book_.quote.ask) {
                book_.quote.ask = top_ask->first;
                send = true;
            }

            if (top_ask->second != book_.quote.ask_qty) {
                book_.quote.ask_qty = top_ask->second;
                send = true;
            }

            if (send) {
                KRYP_LOG(debug, "TOP OF BOOK: {} | {} ::: {} | {} :::: {} | {}", top_bid->second, top_bid->first,
                         top_ask->first,
                         top_ask->second, book_.quote.last, book_.quote.last_qty);
                socket_.template send<krypto::mktdata::Quote>("QUOTE", book_.quote);
            }

        } else {
            KRYP_LOG(debug, "Queueing Incremental");
            incr_queue_.push(incremental);
        }
    }

    template <typename Socket>
    void BookBuilder<Socket>::apply_incremental(int64_t price, int64_t qty, krypto::mktdata::OrderSide side) {
        if (side == OrderSide::BID) {
            if (qty == 0) {
                book_.bids.erase(price);
            } else {
                book_.bids[price] = qty;
            }
        } else if (side == OrderSide::ASK) {
            if (qty == 0) {
                book_.asks.erase(price);
            } else {
                book_.asks[price] = qty;
            }
        } else {
            KRYP_LOG(error, "Something is horribly wrong!!");
        }
    }

    template <typename Socket>
    void BookBuilder<Socket>::handle_snap(nlohmann::json snapshot) {
        if (!snapshot_received_.load()) {

            book_.bids.clear();
            book_.asks.clear();

            auto bids = snapshot.at("bids").get<std::vector<std::vector<std::string>>>();
            auto asks = snapshot.at("asks").get<std::vector<std::vector<std::string>>>();

            for (auto &bid: bids) {

                auto price = krypto::mktdata::convert_price(std::stod(bid[0]));
                auto qty = krypto::mktdata::convert_quantity(std::stod(bid[1]));

                apply_incremental(price, qty, OrderSide::BID);
            }

            for (auto &ask: asks) {

                auto price = krypto::mktdata::convert_price(std::stod(ask[0]));
                auto qty = krypto::mktdata::convert_quantity(std::stod(ask[1]));

                apply_incremental(price, qty, OrderSide::ASK);
            }

            while (!incr_queue_.empty()) {
                nlohmann::json incremental;
                incr_queue_.try_pop(incremental);

                auto incrs = incremental.at("changes").get<std::vector<std::vector<std::string>>>();

                for (auto &incr: incrs) {
                    OrderSide side = incr[0] == "buy" ? OrderSide::BID : OrderSide::ASK;
                    auto price = krypto::mktdata::convert_price(std::stod(incr[1]));
                    auto qty = krypto::mktdata::convert_quantity(std::stod(incr[2]));

                    apply_incremental(price, qty, side);
                }
            }

            snapshot_received_ = true;

        } else {
            // Ignore Update
        }
    }

    template <typename Socket>
    void BookBuilder<Socket>::handle_trade(nlohmann::json trade) {

        auto price = krypto::mktdata::convert_price(
                std::stod(trade.at("price").get<std::string>()));
        auto qty = krypto::mktdata::convert_quantity(
                std::stod(trade.at("size").get<std::string>()));

        book_.quote.last = price;
        book_.quote.last_qty = qty;

        auto side = trade.at("side").get<std::string>() == "sell"?
                Side::BUY : Side::SELL;

        auto trade_id = trade.at("trade_id").get<int64_t>();

        krypto::mktdata::Trade to_send{
            book_.timestamp, 0, price, qty, side, std::to_string(trade_id)};

        socket_.send("TRADE", to_send);
    }

    template <typename Socket>
    BookBuilder<Socket>::~BookBuilder() {
        incr_queue_.clear();
        incr_queue_.abort();
    }

    template <typename Socket>
    const krypto::mktdata::OrderBook &BookBuilder<Socket>::book() const {
        return book_;
    }

}


