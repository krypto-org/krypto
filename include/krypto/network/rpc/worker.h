#pragma once

#include <chrono>
#include <memory>
#include <variant>

#include <flatbuffers/flatbuffers.h>
#include <zmq.hpp>
#include <krypto/network/helper.h>
#include <krypto/logger.h>
#include <krypto/config.h>

namespace krypto::network::rpc {
    template<typename Derived, typename ReceiveParser, typename ResponseVariant, bool Verbose = false>
    class WorkerBase {
    private:
        zmq::context_t context_;
        std::string broker_;
        std::string service_;
        std::unique_ptr<zmq::socket_t> socket_;
        std::atomic_bool running_;

        void connect();

    protected:
        flatbuffers::FlatBufferBuilder fb_builder_;
    public:
        WorkerBase(
                const krypto::Config &config,
                std::string service);

        void send_ready();

        void send_disconnect();

        void start();

        void stop();
    };

    template<typename Derived, typename ReceiveParser, typename ResponseVariant, bool Verbose>
    WorkerBase<Derived, ReceiveParser, ResponseVariant, Verbose>::WorkerBase(
            const krypto::Config &config,
            std::string service) :
            context_{1},
            broker_{config.at<std::string>("/services/rpc/broker/backend/client")},
            service_{std::move(service)},
            running_{false} {
        socket_ = std::make_unique<zmq::socket_t>(context_, ZMQ_DEALER);
    }

    template<typename Derived, typename ReceiveParser, typename ResponseVariant, bool Verbose>
    void WorkerBase<Derived, ReceiveParser, ResponseVariant, Verbose>::connect() {
        int linger = 0;
        socket_->setsockopt(ZMQ_LINGER, &linger, sizeof(linger));

        auto identity = "worker-" + generate_id();

        if constexpr (Verbose) {
            KRYP_LOG(info, "Connecting to broker @ {} with identity {}", broker_, identity);
        }

        socket_->setsockopt(ZMQ_IDENTITY, identity.c_str(), identity.size());
        socket_->connect(broker_.c_str());

        if constexpr (Verbose) {
            KRYP_LOG(info, "Connected to broker @ {}", broker_);
        }

        send_ready();

        if constexpr (Verbose) {
            KRYP_LOG(info, "Sent ready status to broker @ {}", broker_);
        }
    }

    template<typename Derived, typename ReceiveParser, typename ResponseVariant, bool Verbose>
    void WorkerBase<Derived, ReceiveParser, ResponseVariant, Verbose>::send_ready() {
        if constexpr (Verbose) {
            KRYP_LOG(info, "Sending ready status");
        }

        send_empty_frame(*socket_, ZMQ_SNDMORE);
        send_status(*socket_, SocketStatus::READY, ZMQ_SNDMORE);
        send_string(*socket_, service_);
    }

    template<typename Derived, typename ReceiveParser, typename ResponseVariant, bool Verbose>
    void WorkerBase<Derived, ReceiveParser, ResponseVariant, Verbose>::start() {

        connect();

        running_ = true;

        while (running_) {
            zmq::pollitem_t poller = {*socket_, 0, ZMQ_POLLIN, 0};
            zmq::poll(&poller, 1, 1);

            if (poller.revents && ZMQ_POLLIN) {

                recv_empty_frame(*socket_);
                auto address = recv_string(*socket_);

                recv_empty_frame(*socket_);
                auto msg_type = recv_msg_type(*socket_);

                zmq::message_t payload_msg;
                socket_->recv(&payload_msg);

                if constexpr (Verbose) {
                    KRYP_LOG(info, "Received request with payload size {} from {}",
                             payload_msg.size(), address);
                }

                auto payload = ReceiveParser::parse(payload_msg, msg_type);

                auto &derived = static_cast<Derived &>(*this);

                krypto::utils::MsgType result_msg_type = krypto::utils::MsgType::UNDEFINED;

                if (payload.has_value()) {
                    result_msg_type = std::visit([&](auto &&r) -> krypto::utils::MsgType {
                        return derived.process(r);
                    }, payload.value());
                }

                send_empty_frame(*socket_, ZMQ_SNDMORE);
                send_status(*socket_, SocketStatus::REPLY, ZMQ_SNDMORE);
                send_string(*socket_, service_, ZMQ_SNDMORE);
                send_string(*socket_, address, ZMQ_SNDMORE);
                send_empty_frame(*socket_, ZMQ_SNDMORE);

                bool flag = result_msg_type == krypto::utils::MsgType::NO_PAYLOAD;
                send_msg_type(*socket_, result_msg_type, flag ? ZMQ_NULL : ZMQ_SNDMORE);

                if (!flag) {
                    zmq::message_t result_msg(fb_builder_.GetSize());
                    std::memcpy(result_msg.data(), fb_builder_.GetBufferPointer(), fb_builder_.GetSize());
                    socket_->send(result_msg);
                }

                if constexpr (Verbose) {
                    KRYP_LOG(info, "Sent result to {}", address);
                }
            }
        }

        send_disconnect();
    }

    template<typename Derived, typename ReceiveParser, typename ResponseVariant, bool Verbose>
    void WorkerBase<Derived, ReceiveParser, ResponseVariant, Verbose>::stop() {
        running_ = false;
    }

    template<typename Derived, typename ReceiveParser, typename ResponseVariant, bool Verbose>
    void WorkerBase<Derived, ReceiveParser, ResponseVariant, Verbose>::send_disconnect() {
        if constexpr (Verbose) {
            KRYP_LOG(info, "Sending disconnect status");
        }

        send_empty_frame(*socket_, ZMQ_SNDMORE);
        send_status(*socket_, SocketStatus::DISCONNECT, ZMQ_SNDMORE);
        send_string(*socket_, service_);
    }
}