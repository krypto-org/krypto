#pragma once

#include <string>

#include <krypto/utils/common.h>
#include <krypto/serialization/serialization_generated.h>
#include <krypto/utils/types.h>

namespace krypto::utils {
    struct InstrumentRequest {
        krypto::serialization::RequestType request_type;
        static const constexpr krypto::utils::MsgType message_type =
                krypto::utils::MsgType::INSTRUMENT_REQUEST;
    };

    struct InstrumentCacheRefreshRequest {
        static const constexpr krypto::utils::MsgType message_type =
                krypto::utils::MsgType::INSTRUMENT_CACHE_REFRESH_REQUEST;
    };

    struct OrderUpdate final {
        static const constexpr krypto::utils::MsgType message_type =
                krypto::utils::MsgType::ORDER_UPDATE;
        int64_t timestamp;
        std::string order_id;
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
}
