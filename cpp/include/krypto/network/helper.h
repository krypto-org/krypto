#pragma once

#include <string_view>
#include <zmq.hpp>

#include <krypto/utils/common.h>
#include <krypto/utils/types.h>

namespace krypto::network {

    enum class SocketStatus : uint8_t {
        READY = 0x01,
        REQUEST = 0x02,
        REPLY = 0x03,
        HEARTBEAT = 0x04,
        DISCONNECT = 0x05,
    };

    static const constexpr char *SocketStatusNames[] = {
            nullptr,
            "READY",
            "REQUEST",
            "REPLY",
            "HEARTBEAT",
            "DISCONNECT"
    };

    std::string generate_id();

    void connect(zmq::socket_t &socket,
            const std::string& endpoint,
            const std::string& identity);

    void send_string(zmq::socket_t &socket, std::string &&value, zmq::send_flags flags_ = zmq::send_flags::none);

    void send_string(zmq::socket_t &socket, const std::string &value, zmq::send_flags flags_ = zmq::send_flags::none);

    void send_empty_frame(zmq::socket_t &socket, zmq::send_flags flags_ = zmq::send_flags::none);

    void send_status(zmq::socket_t &socket, SocketStatus &&status, zmq::send_flags flags_ = zmq::send_flags::none);

    void send_msg_type(zmq::socket_t &socket, const krypto::utils::MsgType &msg_type, zmq::send_flags flags_ = zmq::send_flags::none);

    void send_fb_buffer(zmq::socket_t &socket,
            const flatbuffers::FlatBufferBuilder& fb_builder, zmq::send_flags flags_ = zmq::send_flags::none);

    std::string recv_string(zmq::socket_t &socket);

    void recv_empty_frame(zmq::socket_t &socket);

    SocketStatus recv_status(zmq::socket_t &socket);

    krypto::utils::MsgType recv_msg_type(zmq::socket_t &socket);
}
