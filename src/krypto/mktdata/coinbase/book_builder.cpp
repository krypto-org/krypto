#include <krypto/mktdata/coinbase/book_builder.h>
#include <krypto/logger.h>

void krypto::mktdata::coinbase::BookBuilder::apply_incremental(double_t price, double_t qty,
                                                               krypto::mktdata::OrderSide side) {
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

krypto::mktdata::coinbase::BookBuilder::BookBuilder() : snapshot_received_{false} {
    incr_queue_.set_capacity(QUEUE_SIZE);

    book_.quote.bid = 0;
    book_.quote.bid_qty = 0;

    book_.quote.ask = std::numeric_limits<double_t>::infinity();
    book_.quote.ask_qty = 0;

    book_.quote.last = 0;
    book_.quote.last_qty = 0;
}

void krypto::mktdata::coinbase::BookBuilder::handle_incr(nlohmann::json incremental) {
    if (snapshot_received_.load()) {

        auto incrs = incremental.at("changes").get<std::vector<std::vector<std::string>>>();

        for (auto &incr: incrs) {
            OrderSide side = incr[0] == "buy" ? OrderSide::BID : OrderSide::ASK;
            auto price = std::stod(incr[1]);
            auto qty = std::stod(incr[2]);

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
            KRYP_LOG(debug, "TOP OF BOOK: {} | {} ::: {} | {} :::: {} | {}", top_bid->second, top_bid->first, top_ask->first,
                     top_ask->second, book_.quote.last, book_.quote.last_qty);

        }

    } else {
        KRYP_LOG(debug, "Queueing Incremental");
        incr_queue_.push(incremental);
    }
}

void krypto::mktdata::coinbase::BookBuilder::handle_snap(nlohmann::json snapshot) {
    if (!snapshot_received_.load()) {

        book_.bids.clear();
        book_.asks.clear();

        auto bids = snapshot.at("bids").get<std::vector<std::vector<std::string>>>();
        auto asks = snapshot.at("asks").get<std::vector<std::vector<std::string>>>();

        for (auto &bid: bids) {

            auto price = std::stod(bid[0]);
            auto qty = std::stod(bid[1]);

            apply_incremental(price, qty, OrderSide::BID);
        }

        for (auto &ask: asks) {

            auto price = std::stod(ask[0]);
            auto qty = std::stod(ask[1]);

            apply_incremental(price, qty, OrderSide::ASK);
        }

        while (!incr_queue_.empty()) {
            nlohmann::json incremental;
            incr_queue_.try_pop(incremental);

            auto incrs = incremental.at("changes").get<std::vector<std::vector<std::string>>>();

            for (auto &incr: incrs) {
                OrderSide side = incr[0] == "buy" ? OrderSide::BID : OrderSide::ASK;
                auto price = std::stod(incr[1]);
                auto qty = std::stod(incr[2]);

                apply_incremental(price, qty, side);
            }
        }

        snapshot_received_ = true;

    } else {
        // Ignore Update
    }
}

void krypto::mktdata::coinbase::BookBuilder::handle_trade(nlohmann::json trade) {
    auto price = std::stod(trade.at("price").get<std::string>());
    auto qty = std::stod(trade.at("size").get<std::string>());

    book_.quote.last = price;
    book_.quote.last_qty = qty;
}

krypto::mktdata::coinbase::BookBuilder::~BookBuilder() {
    incr_queue_.clear();
    incr_queue_.abort();
}
