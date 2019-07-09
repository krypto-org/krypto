#pragma once

#include <cstdint>
#include <cstddef>
#include <chrono>
#include <string>


namespace krypto::network::mdp {
    constexpr const std::string_view MDP_CLIENT = "MDPC01";
    constexpr const std::string_view MDP_WORKER = "MDPW01";

    enum class MdpStatus : uint8_t {
        MDPW_READY = 0x01,
        MDPW_REQUEST = 0x02,
        MDPW_REPLY = 0x03,
        MDPW_HEARTBEAT = 0x04,
        MDPW_DISCONNECT = 0x05,
    };

    static const constexpr char * MDP_COMMANDS [] = {
            nullptr,
            "READY",
            "REQUEST", "REPLY",
            "HEARTBEAT",
            "DISCONNECT"
    };

    std::string generate_id();
}

