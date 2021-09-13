#pragma once

#include <cstdint>
#include <unordered_map>
#include <krypto/utils/enum.h>
#include <string>
#include <cmath>
#include <spdlog/fmt/fmt.h>
#include <ostream>
#include <krypto/serialization/serialization_generated.h>

namespace krypto::utils {

    enum class MsgType : uint8_t {
        UNDEFINED,
        NO_PAYLOAD,
        QUOTE,
        TRADE,
        HEARTBEAT,
        INSTRUMENT_REQUEST,
        INSTRUMENT_RESPONSE,
        INSTRUMENT_CACHE_REFRESH_REQUEST,
        ORDER,
        ORDER_REQUEST,
        ORDER_CANCEL_REQUEST,
        ORDER_REPLACE_REQUEST,
        ORDER_UPDATE,
        POSITION,
        RISK_SUMMARY,
        THEO,
        SIZE
    };

    static constexpr const char *MsgTypeNames[enum_size<MsgType>()] = {
            "",
            "AA",
            "MQ",
            "MT",
            "HH",
            "IQ",
            "IS",
            "IC",
            "O_",
            "OO",
            "OC",
            "OR",
            "OU",
            "PP",
            "PR",
            "PT"
    };

    static constexpr const MsgType MsgTypes[enum_size<MsgType>()] = {
            MsgType::UNDEFINED,
            MsgType::NO_PAYLOAD,
            MsgType::QUOTE,
            MsgType::TRADE,
            MsgType::HEARTBEAT,
            MsgType::INSTRUMENT_REQUEST,
            MsgType::INSTRUMENT_RESPONSE,
            MsgType::INSTRUMENT_CACHE_REFRESH_REQUEST,
            MsgType::ORDER,
            MsgType::ORDER_REQUEST,
            MsgType::ORDER_CANCEL_REQUEST,
            MsgType::ORDER_REPLACE_REQUEST,
            MsgType::ORDER_UPDATE,
            MsgType::POSITION,
            MsgType::RISK_SUMMARY,
            MsgType::THEO
    };

    constexpr const int64_t exchange_base = 100;

    constexpr auto exchange_id(const krypto::serialization::Exchange et) {
        return static_cast<int64_t>(et) + exchange_base + 1;
    }

    constexpr int64_t instrument_id(
            const krypto::serialization::Exchange et,
            const krypto::serialization::Currency base,
            const krypto::serialization::Currency quote) {
        return exchange_id(et) * 100000000 + static_cast<int64_t>(base) * 10000 +
               static_cast<int64_t>(quote);
    }

    std::string instrument_symbol(const std::string &base, const std::string &quote);

    std::string create_topic(MsgType msg_type, int64_t id);

    std::string create_topic(MsgType msg_type, krypto::serialization::Exchange exchange);

    std::unordered_map<std::string, krypto::serialization::Currency> name_to_currency() noexcept;

    std::unordered_map<std::string, MsgType> name_to_msg_type();
}
