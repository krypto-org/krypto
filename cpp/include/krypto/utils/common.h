#pragma once

#include <cstdint>
#include <unordered_map>
#include <krypto/utils/enum.h>
#include <string>
#include <cmath>
#include <spdlog/fmt/fmt.h>
#include <ostream>
#include <krypto/utils/types.h>
#include <krypto/serialization/serialization_generated.h>

namespace krypto::utils {
    constexpr const uint64_t exchange_base = 100;

    constexpr auto exchange_id(const krypto::serialization::Exchange et) {
        return static_cast<uint64_t>(et) + exchange_base + 1;
    }

    constexpr int64_t instrument_id(
            const krypto::serialization::Exchange et,
            const krypto::serialization::Currency base,
            const krypto::serialization::Currency quote) {
        return exchange_id(et) * 100000000 + static_cast<int64_t>(base) * 10000 +
               static_cast<int64_t>(quote);
    }

    std::string instrument_symbol(const std::string& base, const std::string& quote);

    std::string create_topic(MsgType msg_type, int64_t id);

    std::unordered_map<std::string, krypto::serialization::Currency> name_to_currency();

    std::unordered_map<std::string, krypto::utils::MsgType > name_to_msg_type();
}
