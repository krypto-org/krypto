#pragma once

#include <variant>
#include <zmq.hpp>

#include <krypto/utils/common.h>
#include <krypto/mktdata/protocol.h>
#include <krypto/serialization/serialization_generated.h>

namespace krypto::orders {

    enum class OrderStatus : uint8_t {
        UNKNOWN,
        IN_FLIGHT,
        NEW,
        ACCEPTED,
        CANCELLED,
        REPLACED,
        REJECTED,
        FILLED,
        PARTIALLY_FILLED
    };

    static const constexpr char *OrderStatusNames[] = {
            nullptr,
            "IN_FLIGHT",
            "NEW",
            "ACCEPTED",
            "CANCELLED",
            "REPLACED",
            "REJECTED",
            "FILLED",
            "PARTIALLY_FILLED"
    };

    template<OrderStatus status>
    const constexpr char *order_status_name() {
        return OrderStatusNames[static_cast<uint8_t>(status)];
    }

    krypto::serialization::OrderStatus serialize(OrderStatus status);

    struct OrderUpdate final {
        uint64_t timestamp;
        std::string order_id;
        OrderStatus status;
        uint64_t filled_quantity;
    };

    struct OrderRequest final {
        uint64_t timestamp;
        uint64_t security_id;
        uint64_t price;
        uint64_t quantity;
        krypto::mktdata::Side side;
        std::string order_id;
    };

    struct OrderCancelRequest final {
        uint64_t timestamp;
        std::string order_id;
    };

    struct OrderReplaceRequest final {
        uint64_t timestamp;
        std::string order_id;
        uint64_t price;
        uint64_t quantity;
        krypto::mktdata::Side side;
    };

    struct ClientParser final {
        using receive_variant_t = std::variant<
                const krypto::serialization::OrderUpdate *>;

        static std::optional<receive_variant_t> parse(
                const zmq::message_t &msg, krypto::utils::MsgType msg_type);
    };

    struct ServerParser final {
        using receive_variant_t = std::variant<
                const krypto::serialization::OrderRequest *,
                const krypto::serialization::OrderCancelRequest *,
                const krypto::serialization::OrderReplaceRequest *>;

        static std::optional<receive_variant_t> parse(
                const zmq::message_t &msg, krypto::utils::MsgType msg_type);
    };
}
