#pragma once

#include <cstdint>
#include <unordered_map>
#include <krypto/utils/enum.h>

namespace krypto::common {
    constexpr const uint64_t exchange_base = 100;

    enum struct ExchangeType : uint8_t {
        COINBASE,
        SIZE
    };

    using ExchangeTypeEnum = krypto::utils::Enum<ExchangeType>;

    constexpr auto exchange_id(const ExchangeType et) {
        return static_cast<uint64_t>(et) + exchange_base + 1;
    }
}
