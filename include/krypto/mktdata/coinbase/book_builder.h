#pragma once

#include <atomic>
#include <queue>

#include <tbb/concurrent_queue.h>
#include <nlohmann/json.hpp>

#include <krypto/network/mktdata/top_of_book.h>
#include <krypto/mktdata/convert.h>
#include <krypto/instruments/client.h>
#include <krypto/utils/date_time.h>
#include <krypto/mktdata/protocol.h>


namespace krypto::mktdata::coinbase {
    template<bool Verbose = false>
    class BookBuilder final {
    private:
        std::unordered_map<std::string, uint64_t> id_by_symbol_;
        std::unordered_map<std::string, std::unique_ptr<OrderBook>> books;
        std::unordered_map<std::string, std::queue<nlohmann::json>> incr_queue_;
        std::unordered_map<std::string, bool> snapshot_received_;

        krypto::network::mktdata::TopOfBookPublisher publisher_;

        void apply_incremental(const std::string &symbol, int64_t price, int64_t qty, OrderSide side);
        int64_t parse_time(const std::string& ts);
    public:
        BookBuilder(zmq::context_t& context, const krypto::Config &config);

        ~BookBuilder();

        BookBuilder(const BookBuilder<Verbose> &other) = default;

        BookBuilder(BookBuilder<Verbose> &&other) noexcept = default;

        BookBuilder<Verbose> &operator=(const BookBuilder<Verbose> &other) = default;

        BookBuilder<Verbose> &operator=(BookBuilder<Verbose> &&other) noexcept = default;

        void handle_incr(nlohmann::json incremental);

        void handle_snap(nlohmann::json snapshot);

        void handle_trade(nlohmann::json trade);

        void handle_heartbeat(nlohmann::json hb);
    };

    template<bool Verbose>
    BookBuilder<Verbose>::BookBuilder(zmq::context_t& context, const krypto::Config &config) :
            publisher_{context, config.at<std::string>("/services/publisher/mktdata/proxy/frontend/client")} {

        krypto::instruments::InstrumentClient client{context, config};
        auto instruments = client.query_all(1000);

        if (instruments.empty()) {
            KRYP_LOG(info, "No instruments received");
        }

        std::for_each(instruments.cbegin(), instruments.cend(), [=](auto &&instr) {
            if (instr.exchange == krypto::utils::ExchangeType::COINBASE) {
                id_by_symbol_[instr.exchange_symbol] = instr.id;
                books[instr.exchange_symbol] = std::make_unique<OrderBook>();
                books.at(instr.exchange_symbol)->security_id = instr.id;
                books.at(instr.exchange_symbol)->quote = {0, instr.id, 0, std::numeric_limits<int64_t>::max(), 0, 0, 0,
                                                          0};

                snapshot_received_[instr.exchange_symbol] = false;

                incr_queue_[instr.exchange_symbol] = {};
            }
        });

        publisher_.connect();
    }

    template<bool Verbose>
    void BookBuilder<Verbose>::handle_incr(nlohmann::json incremental) {
        auto symbol = incremental.at("product_id").get<std::string>();

        if (snapshot_received_.at(symbol)) {

            auto incrs = incremental.at("changes").get<std::vector<std::vector<std::string>>>();

            for (auto &incr: incrs) {
                OrderSide side = incr[0] == "buy" ? OrderSide::BID : OrderSide::ASK;
                auto price = krypto::mktdata::convert_price(std::stod(incr[1]));
                auto qty = krypto::mktdata::convert_quantity(std::stod(incr[2]));

                apply_incremental(symbol, price, qty, side);
            }

            auto top_bid = std::begin(books.at(symbol)->bids);
            auto top_ask = std::begin(books.at(symbol)->asks);

            bool send = false;

            if (top_bid->first != books.at(symbol)->quote.bid) {
                books.at(symbol)->quote.bid = top_bid->first;
                send = true;
            }

            if (top_bid->second != books.at(symbol)->quote.bid_qty) {
                books.at(symbol)->quote.bid_qty = top_bid->second;
                send = true;
            }

            if (top_ask->first != books.at(symbol)->quote.ask) {
                books.at(symbol)->quote.ask = top_ask->first;
                send = true;
            }

            if (top_ask->second != books.at(symbol)->quote.ask_qty) {
                books.at(symbol)->quote.ask_qty = top_ask->second;
                send = true;
            }

            auto current_time = krypto::utils::current_time_in_nanoseconds();

            books.at(symbol)->timestamp = current_time;
            books.at(symbol)->quote.timestamp = current_time;

            if (send) {
                auto id = id_by_symbol_.at(symbol);
                auto topic = krypto::utils::create_topic(krypto::utils::MsgType::QUOTE, id);
                publisher_.send<krypto::mktdata::Quote>(topic, books.at(symbol)->quote);
            }

        } else {
            KRYP_LOG(debug, "Queueing Incremental");
            incr_queue_.at(symbol).push(incremental);
        }
    }

    template<bool Verbose>
    void BookBuilder<Verbose>::apply_incremental(
            const std::string &symbol,
            int64_t price,
            int64_t qty,
            krypto::mktdata::OrderSide side) {
        if (side == OrderSide::BID) {
            if (qty == 0) {
                books.at(symbol)->bids.erase(price);
            } else {
                books.at(symbol)->bids[price] = qty;
            }
        } else if (side == OrderSide::ASK) {
            if (qty == 0) {
                books.at(symbol)->asks.erase(price);
            } else {
                books.at(symbol)->asks[price] = qty;
            }
        } else {
            KRYP_LOG(error, "Something is horribly wrong!!");
        }
    }

    template<bool Verbose>
    void BookBuilder<Verbose>::handle_snap(nlohmann::json snapshot) {
        auto symbol = snapshot.at("product_id").get<std::string>();

        if (!snapshot_received_.at(symbol)) {

            books.at(symbol)->bids.clear();
            books.at(symbol)->asks.clear();

            auto bids = snapshot.at("bids").get<std::vector<std::vector<std::string>>>();
            auto asks = snapshot.at("asks").get<std::vector<std::vector<std::string>>>();

            for (auto &bid: bids) {

                auto price = krypto::mktdata::convert_price(std::stod(bid[0]));
                auto qty = krypto::mktdata::convert_quantity(std::stod(bid[1]));

                apply_incremental(symbol, price, qty, OrderSide::BID);
            }

            for (auto &ask: asks) {

                auto price = krypto::mktdata::convert_price(std::stod(ask[0]));
                auto qty = krypto::mktdata::convert_quantity(std::stod(ask[1]));

                apply_incremental(symbol, price, qty, OrderSide::ASK);
            }

            while (!incr_queue_.at(symbol).empty()) {
                nlohmann::json incremental = incr_queue_.at(symbol).front();
                incr_queue_.at(symbol).pop();

                auto incrs = incremental.at("changes").get<std::vector<std::vector<std::string>>>();

                for (auto &incr: incrs) {
                    OrderSide side = incr[0] == "buy" ? OrderSide::BID : OrderSide::ASK;
                    auto price = krypto::mktdata::convert_price(std::stod(incr[1]));
                    auto qty = krypto::mktdata::convert_quantity(std::stod(incr[2]));

                    apply_incremental(symbol, price, qty, side);
                }
            }

            snapshot_received_[symbol] = true;

        } else {
            // Ignore Update
        }
    }

    template<bool Verbose>
    void BookBuilder<Verbose>::handle_trade(nlohmann::json trade) {
        auto symbol = trade.at("product_id").get<std::string>();

        books.at(symbol)->timestamp = krypto::utils::current_time_in_nanoseconds();

        auto id = id_by_symbol_.at(symbol);

        auto price = krypto::mktdata::convert_price(
                std::stod(trade.at("price").get<std::string>()));
        auto qty = krypto::mktdata::convert_quantity(
                std::stod(trade.at("size").get<std::string>()));

        books.at(symbol)->quote.last = price;
        books.at(symbol)->quote.last_qty = qty;

        auto side = trade.at("side").get<std::string>() == "sell" ?
                    Side::BUY : Side::SELL;

        auto trade_id = trade.at("trade_id").get<int64_t>();

        krypto::mktdata::Trade to_send{
                books.at(symbol)->timestamp, id, price, qty, side, std::to_string(trade_id)};

        auto topic = krypto::utils::create_topic(krypto::utils::MsgType::TRADE, id);

        publisher_.send(topic, to_send);
    }

    template<bool Verbose>
    BookBuilder<Verbose>::~BookBuilder() {
        incr_queue_.clear();
    }

    template<bool Verbose>
    int64_t BookBuilder<Verbose>::parse_time(const std::string& ts) {
        return krypto::utils::parse_8601(ts).count();
    }

    template<bool Verbose>
    void BookBuilder<Verbose>::handle_heartbeat(nlohmann::json hb) {
        //{"last_trade_id":0,"product_id":"ZEC-BTC","sequence":25841202,
        // "time":"2019-09-05T23:50:54.686991357Z","type":"heartbeat"}
        auto symbol = hb.at("product_id").get<std::string>();
        auto id = id_by_symbol_.at(symbol);
        krypto::utils::Heartbeat to_send{krypto::utils::current_time_in_nanoseconds(), id};
        auto topic = krypto::utils::create_topic(krypto::utils::MsgType::HEARTBEAT, id);
        publisher_.send(topic, to_send);
    }
}


