#pragma once

#include <cstdint>
#include <cmath>
#include <map>
#include <string>
#include <ostream>
#include <krypto/utils/enum.h>
#include <krypto/serialization/serialization_generated.h>
#include <krypto/utils/common.h>


namespace krypto::utils {
    using price_t = int64_t;
    using quantity_t = int64_t;
    using secid_t = int64_t;
    using timestamp_t = int64_t;
    using seqnum_t = int64_t;

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
        bool active;
        bool sandbox_enabled;

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

    struct Fill final {
        int64_t security_id;
        std::string order_id;
        krypto::serialization::Side side;
        double_t price;
        double_t quantity;
        double_t remaining_quantity;
        std::string trader;
        std::string book;

        Fill(int64_t securityId, const std::string &orderId, serialization::Side side, double_t price,
             double_t quantity, double_t remainingQuantity, const std::string &trader, const std::string &book);

        friend std::ostream &operator<<(std::ostream &os, const Fill &fill);
    };

    struct Order final {
        std::string cl_order_id;
        int64_t security_id;
        std::string symbol;
        double_t price;
        double_t size;
        krypto::serialization::Side side;
        krypto::serialization::TimeInForce tif;
        std::string trader;
        std::string book;
        std::string order_id;
        int64_t last_update_timestamp;
        krypto::serialization::OrderStatus status;
        double_t filled_quantity;
        double_t fees;
        std::vector<Fill> fills;

        Order(std::string cl_order_id, int64_t security_id, const std::string &symbol, double_t price,
              double_t size, serialization::Side side, serialization::TimeInForce tif, const std::string &trader,
              const std::string &book);

        friend std::ostream &operator<<(std::ostream &os, const Order &order);
    };

    struct OrderUpdate final {
        static const constexpr krypto::utils::MsgType message_type =
                krypto::utils::MsgType::ORDER_UPDATE;
        int64_t timestamp;
        std::string order_id;
        std::string exchange_order_id;
        krypto::serialization::OrderStatus status;
        int64_t filled_quantity;
    };

    struct OrderRequest final {
        static const constexpr krypto::utils::MsgType message_type =
                krypto::utils::MsgType::ORDER_REQUEST;
        int64_t timestamp;
        int64_t security_id;
        int64_t price;
        int64_t quantity;
        krypto::serialization::Side side;
        std::string order_id;
        krypto::serialization::TimeInForce tif;
    };

    struct OrderCancelRequest final {
        static const constexpr krypto::utils::MsgType message_type =
                krypto::utils::MsgType::ORDER_CANCEL_REQUEST;
        int64_t timestamp;
        std::string order_id;
    };

    struct OrderReplaceRequest final {
        static const constexpr krypto::utils::MsgType message_type =
                krypto::utils::MsgType::ORDER_REPLACE_REQUEST;
        int64_t timestamp;
        std::string order_id;
        int64_t price;
        int64_t quantity;
        krypto::serialization::Side side;
    };

    struct TheoreticalSnapshot final {
        static const constexpr krypto::utils::MsgType message_type =
                krypto::utils::MsgType::THEO;
        int64_t timestamp;
        int64_t security_id;
        double_t price;
        double_t adjusted_price;
        double_t error;
        double_t mm_base_bid;
        double_t mm_base_ask;
        double_t mm_base_spread;
        double_t bid_liquidity;
        double_t ask_liquidity;
        bool valid;

        friend std::ostream &operator<<(std::ostream &os, const TheoreticalSnapshot &snapshot) {
            os << "security_id: " << snapshot.security_id << " price: " << snapshot.price << " adjusted_price: "
               << snapshot.adjusted_price << " error: " << snapshot.error << " mm_base_bid: " << snapshot.mm_base_bid
               << " mm_base_ask: " << snapshot.mm_base_ask << " mm_base_spread: " << snapshot.mm_base_spread
               << " bid_liquidity: " << snapshot.bid_liquidity << " ask_liquidity: " << snapshot.ask_liquidity
               << " valid: " << snapshot.valid;
            return os;
        }
    };

    struct Position final {
        int64_t timestamp;
        int64_t security_id;
        double_t position;
        double_t pnl;
        double_t mark_price;
    };

    class MessageTypeData {
    public:
        static const std::unordered_map<std::string, krypto::utils::MsgType> msg_type_reference;
    };

}