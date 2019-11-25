#pragma once

#include <variant>
#include <zmq.hpp>

#include <krypto/utils/common.h>
#include <krypto/mktdata/protocol.h>
#include <krypto/serialization/serialization_generated.h>
#include <krypto/utils/types.h>

namespace krypto::orders {
    struct ServerParser final {
        using receive_variant_t = std::variant<
                const krypto::serialization::OrderRequest *,
                const krypto::serialization::OrderCancelRequest *,
                const krypto::serialization::OrderReplaceRequest *>;

        static std::optional<receive_variant_t> parse(
                const zmq::message_t &msg, krypto::utils::MsgType msg_type);
    };
}
