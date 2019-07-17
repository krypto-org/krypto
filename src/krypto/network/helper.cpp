#include <krypto/network/helper.h>
#include <krypto/logger.h>

namespace krypto::network {

    std::string generate_id() {
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
        return std::to_string(nanoseconds.count());
    }

    void send_string(zmq::socket_t &socket, std::string &&value, int flags_) {
        zmq::message_t message(value.size());
        std::memcpy(message.data(), value.data(), value.size());
        socket.send(message, flags_);
    }

    void send_string(zmq::socket_t &socket, const std::string &value, int flags_) {
        zmq::message_t message(value.size());
        std::memcpy(message.data(), value.data(), value.size());
        socket.send(message, flags_);
    }

    void send_empty_frame(zmq::socket_t &socket, int flags_) {
        std::string empty;
        zmq::message_t empty_msq(empty.size());
        std::memcpy(empty_msq.data(), empty.data(), empty.size());
        socket.send(empty_msq, flags_);
    }

    void send_status(zmq::socket_t &socket, SocketStatus &&status, int flags_) {
        zmq::message_t message(1);
        std::memcpy(message.data(), &status, 1);
        socket.send(message, flags_);
    }

    std::string recv_string(zmq::socket_t &socket) {
        zmq::message_t msg;
        socket.recv(&msg);
        auto result = std::string(static_cast<char *>(msg.data()), msg.size());
        return result;
    }

    void recv_empty_frame(zmq::socket_t &socket) {
        zmq::message_t empty_msq;
        socket.recv(&empty_msq);

        bool is_empty = empty_msq.size() == 0;

        if (!is_empty) {
            auto result = std::string(static_cast<char *>(empty_msq.data()), empty_msq.size());
            KRYP_LOG(error, "Expected empty message. Received non-empty message. {}", result);
        }

        assert(is_empty);
    }

    SocketStatus recv_status(zmq::socket_t &socket) {
        zmq::message_t msg;
        socket.recv(&msg);
        auto result = static_cast<SocketStatus *>(msg.data());
        return *result;
    }
}
