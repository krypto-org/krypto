#pragma once

#include <cstdint>
#include <cmath>
#include <map>
#include <string>
#include <ostream>
#include <krypto/utils/enum.h>
#include <krypto/serialization/serialization_generated.h>


namespace krypto::utils {
    using price_t = int64_t;
    using quantity_t = int64_t;
    using secid_t = int64_t;
    using timestamp_t = int64_t;
    using seqnum_t = int64_t;

    enum class MsgType : uint8_t {
        UNDEFINED,
        NO_PAYLOAD,
        QUOTE,
        TRADE,
        HEARTBEAT,
        INSTRUMENT_REQUEST,
        INSTRUMENT_RESPONSE,
        INSTRUMENT_CACHE_REFRESH_REQUEST,
        ORDER,
        ORDER_REQUEST,
        ORDER_CANCEL_REQUEST,
        ORDER_REPLACE_REQUEST,
        ORDER_UPDATE,
        POSITION,
        RISK_SUMMARY,
        THEO,
        SIZE
    };

    static constexpr const char *MsgTypeNames[enum_size<MsgType>()] = {
            "",
            "AA",
            "MQ",
            "MT",
            "HH",
            "IQ",
            "IS",
            "IC",
            "O_",
            "OO",
            "OC",
            "OR",
            "OU",
            "PP",
            "PR",
            "PT"
    };

    static constexpr const MsgType MsgTypes[enum_size<MsgType>()] = {
            MsgType::UNDEFINED,
            MsgType::NO_PAYLOAD,
            MsgType::QUOTE,
            MsgType::TRADE,
            MsgType::HEARTBEAT,
            MsgType::INSTRUMENT_REQUEST,
            MsgType::INSTRUMENT_RESPONSE,
            MsgType::INSTRUMENT_CACHE_REFRESH_REQUEST,
            MsgType::ORDER,
            MsgType::ORDER_REQUEST,
            MsgType::ORDER_CANCEL_REQUEST,
            MsgType::ORDER_REPLACE_REQUEST,
            MsgType::ORDER_UPDATE,
            MsgType::POSITION,
            MsgType::RISK_SUMMARY,
            MsgType::THEO
    };

    struct Heartbeat {
        timestamp_t timestamp;
        secid_t security_id;
    };

    struct Instrument {
        secid_t id;
        krypto::serialization::InstrumentType inst_type;
        std::string symbol;
        krypto::serialization::Exchange exchange;
        std::string exchange_symbol;
        double_t tick_size;
        double_t min_size;
        double_t max_size;
        krypto::serialization::Currency crypto_base;
        krypto::serialization::Currency crypto_quote;

        friend std::ostream &operator<<(std::ostream &os, const Instrument &instrument) {
            os << "id: " << instrument.id << " symbol: " << instrument.symbol
               << " exchange_symbol: " << instrument.exchange_symbol
               << " tick_size: " << instrument.tick_size << " min_size: " << instrument.min_size << " max_size: "
               << instrument.max_size;
            return os;
        }
    };

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
        int64_t security_id;
        int64_t timestamp;
        std::map<int64_t, int64_t, std::greater<>> bids; // Decreasing Order
        std::map<int64_t, int64_t> asks;
        Quote quote;
    };
}