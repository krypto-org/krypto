#pragma once

#include <string_view>
#include <zmq.hpp>

namespace krypto::network {

    enum class SocketStatus : uint8_t {
        READY = 0x01,
        REQUEST = 0x02,
        REPLY = 0x03,
        HEARTBEAT = 0x04,
        DISCONNECT = 0x05,
    };

    static const constexpr char * MDP_COMMANDS [] = {
            nullptr,
            "READY",
            "REQUEST",
            "REPLY",
            "HEARTBEAT",
            "DISCONNECT"
    };

    std::string generate_id();

    void send_string(zmq::socket_t& socket, std::string&& value, int flags_);

    void send_string(zmq::socket_t& socket, const std::string& value, int flags_);

    void send_empty_frame(zmq::socket_t& socket, int flags_);

    void send_status(zmq::socket_t& socket, SocketStatus &&status, int flags_);

    std::string recv_string(zmq::socket_t& socket);

    void recv_empty_frame(zmq::socket_t& socket);

    SocketStatus recv_status(zmq::socket_t& socket);
}
