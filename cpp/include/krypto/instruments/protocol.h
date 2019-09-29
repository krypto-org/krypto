#pragma once

#include <variant>
#include <zmq.hpp>

#include <krypto/utils/common.h>
#include <krypto/serialization/serialization_generated.h>
#include <krypto/utils/types.h>

namespace krypto::instruments {
    struct ClientParser final {
        using receive_variant_t = std::variant<
                const krypto::serialization::InstrumentResponse *>;

        static std::optional<receive_variant_t> parse(
                const zmq::message_t &msg, krypto::utils::MsgType msg_type);
    };

    struct ServerParser final {
        using receive_variant_t = std::variant<
                const krypto::serialization::InstrumentRequest *,
                const krypto::serialization::InstrumentCacheRefreshRequest * >;

        static std::optional<receive_variant_t> parse(
                const zmq::message_t &msg, krypto::utils::MsgType msg_type);
    };
}