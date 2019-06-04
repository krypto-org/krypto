#pragma once

#include <map>
#include <cmath>
#include <ostream>

namespace krypto::mktdata {
    enum class OrderSide : int8_t {
        BID = 1, ASK = 2, UNKNOWN = 0
    };

    enum class Side : int8_t {
        BUY = 1, SELL = 2, UNKNOWN = 0
    };

    struct Quote {
        int64_t timestamp;
        int64_t security_id;
        int64_t bid;
        int64_t ask;
        int64_t bid_qty;
        int64_t ask_qty;
        int64_t last;
        int64_t last_qty;
    };

    struct Trade {
        int64_t timestamp;
        int64_t security_id;
        int64_t price;
        int64_t quantity;
        Side side; // Taker Side
        std::string trade_id;
    };

    struct OrderBook {
        int64_t timestamp;
        int64_t security_id;
        std::map<int64_t, int64_t, std::greater<>> bids; // Decresing Order
        std::map<int64_t, int64_t> asks;
        Quote quote;
    };
}
