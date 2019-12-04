#pragma once

#include <variant>
#include <zmq.hpp>

#include <krypto/utils/common.h>
#include <krypto/serialization/serialization_generated.h>
#include <map>
#include <cmath>
#include <ostream>
#include <krypto/utils/types.h>

namespace krypto::mktdata {
    struct Parser final {
        using receive_variant_t = std::variant<
                const krypto::serialization::Quote *,
                const krypto::serialization::Trade *,
                const krypto::serialization::Heartbeat *,
                const krypto::serialization::OrderUpdate *>;

        static std::optional<receive_variant_t> parse(const zmq::message_t &msg,
                                                      krypto::utils::MsgType msg_type);
    };
}