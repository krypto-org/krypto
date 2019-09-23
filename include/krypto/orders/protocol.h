#pragma once

#include <variant>
#include <zmq.hpp>

#include <krypto/utils/common.h>
#include <krypto/mktdata/protocol.h>
#include <krypto/serialization/serialization_generated.h>

namespace krypto::orders {

    struct OrderUpdate final {
        static const constexpr krypto::utils::MsgType message_type =
                krypto::utils::MsgType::ORDER_UPDATE;
        uint64_t timestamp;
        std::string order_id;
        krypto::serialization::OrderStatus status;
        int64_t filled_quantity;
    };

    struct OrderRequest final {
        static const constexpr krypto::utils::MsgType message_type =
                krypto::utils::MsgType::ORDER_REQUEST;
        uint64_t timestamp;
        uint64_t security_id;
        int64_t price;
        int64_t quantity;
        krypto::serialization::Side side;
        std::string order_id;
        krypto::serialization::TimeInForce tif;
    };

    struct OrderCancelRequest final {
        static const constexpr krypto::utils::MsgType message_type =
                krypto::utils::MsgType::ORDER_CANCEL_REQUEST;
        uint64_t timestamp;
        std::string order_id;
    };

    struct OrderReplaceRequest final {
        static const constexpr krypto::utils::MsgType message_type =
                krypto::utils::MsgType::ORDER_REPLACE_REQUEST;
        uint64_t timestamp;
        std::string order_id;
        int64_t price;
        int64_t quantity;
        krypto::serialization::Side side;
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
