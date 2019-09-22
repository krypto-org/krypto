#pragma once

#include <variant>
#include <zmq.hpp>

#include <krypto/utils/common.h>
#include <krypto/serialization/serialization_generated.h>
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
        uint64_t timestamp;
        uint64_t security_id;
        int64_t bid;
        int64_t ask;
        int64_t bid_qty;
        int64_t ask_qty;
        int64_t last;
        int64_t last_qty;
    };

    struct Trade {
        uint64_t timestamp;
        uint64_t security_id;
        int64_t price;
        int64_t quantity;
        Side side; // Taker Side
        std::string trade_id;
    };

    struct OrderBook {
        uint64_t security_id;
        uint64_t timestamp;
        std::map<int64_t, int64_t, std::greater<>> bids; // Decreasing Order
        std::map<int64_t, int64_t> asks;
        Quote quote;
    };

    struct Parser final {
        using receive_variant_t = std::variant<
                const krypto::serialization::Quote *,
                const krypto::serialization::Trade *,
                const krypto::serialization::Heartbeat *>;

        static std::optional<receive_variant_t> parse(const zmq::message_t &msg,
                                                      krypto::utils::MsgType msg_type);
    };
}