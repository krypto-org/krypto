#include <krypto/network/helper.h>
#include <krypto/logger.h>
#include <krypto/utils/types.h>

namespace krypto::network {

    std::string generate_id() {
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
        return std::to_string(nanoseconds.count());
    }

    void connect(zmq::socket_t &socket, const std::string &endpoint, const std::string &identity) {
        int linger = 0;
        KRYP_LOG(info, "Setting identity : {}", identity);
        socket.set(zmq::sockopt::linger, linger);
        socket.set(zmq::sockopt::routing_id, identity);
        socket.connect(endpoint);
    }

    void send_string(zmq::socket_t &socket, std::string &&value, zmq::send_flags flags_) {
        zmq::message_t message(value.size());
        std::memcpy(message.data(), value.data(), value.size());
        socket.send(message, flags_);
    }

    void send_string(zmq::socket_t &socket, const std::string &value, zmq::send_flags flags_) {
        zmq::message_t message(value.size());
        std::memcpy(message.data(), value.data(), value.size());
        socket.send(message, flags_);
    }

    void send_empty_frame(zmq::socket_t &socket, zmq::send_flags flags_) {
        std::string empty;
        zmq::message_t empty_msq(empty.size());
        std::memcpy(empty_msq.data(), empty.data(), empty.size());
        socket.send(empty_msq, flags_);
    }

    void send_status(zmq::socket_t &socket, SocketStatus &&status, zmq::send_flags flags_) {
        zmq::message_t message(1);
        std::memcpy(message.data(), &status, 1);
        socket.send(message, flags_);
    }

    void send_msg_type(zmq::socket_t &socket, const krypto::utils::MsgType &msg_type, zmq::send_flags flags_) {
        zmq::message_t message(1);
        std::memcpy(message.data(), &msg_type, 1);
        socket.send(message, flags_);
    }

    std::string recv_string(zmq::socket_t &socket) {
        zmq::message_t msg;
        const auto msg_size = socket.recv(msg);
        if (!msg_size.has_value()) {
            KRYP_LOG(error, "Message frame is empty");
        }
        auto result = std::string(static_cast<char *>(msg.data()), msg.size());
        return result;
    }

    void recv_empty_frame(zmq::socket_t &socket) {
        zmq::message_t empty_msq;
        const auto msg_size = socket.recv(empty_msq);

        if (!msg_size.has_value()) {
            return;
        }

        if (!empty_msq.empty()) {
            auto result = std::string(static_cast<char *>(empty_msq.data()), empty_msq.size());
            KRYP_LOG(error, "Expected empty message. Received non-empty message. {}", result);
        }

        assert(empty_msq.empty());
    }

    SocketStatus recv_status(zmq::socket_t &socket) {
        zmq::message_t msg;
        const auto msg_size = socket.recv(msg);
        if (!msg_size.has_value()) {
            KRYP_LOG(error, "SocketStatus not found. Frame is empty");
        }
        auto result = static_cast<SocketStatus *>(msg.data());
        return *result;
    }

    krypto::utils::MsgType recv_msg_type(zmq::socket_t &socket) {
        zmq::message_t msg;
        const auto msg_size = socket.recv(msg);
        if (!msg_size.has_value()) {
            KRYP_LOG(error, "MsgType message not found. Frame is empty");
        }
        auto result = static_cast<krypto::utils::MsgType *>(msg.data());
        return *result;
    }

    void send_fb_buffer(zmq::socket_t &socket,
                        const flatbuffers::FlatBufferBuilder &fb_builder, zmq::send_flags flags_) {
        zmq::message_t request_msg(fb_builder.GetSize());
        std::memcpy(request_msg.data(),
                    fb_builder.GetBufferPointer(),
                    fb_builder.GetSize());
        socket.send(request_msg, flags_);
    }
}
