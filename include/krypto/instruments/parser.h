#pragma once

#include <variant>
#include <zmq.hpp>

#include <krypto/utils/common.h>
#include <krypto/serialization/serialization_generated.h>

namespace krypto::instruments {
    struct Parser final {
        using receive_variant_t = std::variant<
                const krypto::serialization::InstrumentResponse *>;

        static std::optional<receive_variant_t> parse(
                const zmq::message_t &msg, krypto::utils::MsgType msg_type);
    };
}