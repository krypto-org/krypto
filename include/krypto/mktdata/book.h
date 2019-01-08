#pragma once

#include <map>
#include <cmath>

namespace krypto::mktdata {
    enum class OrderSide : int8_t {
        BID = 1, ASK = 2, UNKNOWN = 0
    };

    struct Quote {
        double_t bid;
        double_t ask;
        double_t bid_qty;
        double_t ask_qty;
        double_t last;
        double_t last_qty;
        int64_t timestamp;
    };

    struct OrderBook {
        std::map<double_t, double_t, std::greater<>> bids; // Decresing Order
        std::map<double_t, double_t> asks;
        int64_t timestamp;
        Quote quote;
    };
}
