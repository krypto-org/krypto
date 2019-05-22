#pragma once

#include <map>
#include <cmath>

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
        double_t bid;
        double_t ask;
        double_t bid_qty;
        double_t ask_qty;
        double_t last;
        double_t last_qty;
    };

    struct Trade {
        int64_t timestamp;
        int64_t security_id;
        int64_t price;
        int64_t quantity;
        Side side; // Aggressor Side
        std::string order_id;
    };

    struct OrderBook {
        int64_t timestamp;
        int64_t security_id;
        std::map<double_t, double_t, std::greater<>> bids; // Decresing Order
        std::map<double_t, double_t> asks;
        Quote quote;
    };
}
