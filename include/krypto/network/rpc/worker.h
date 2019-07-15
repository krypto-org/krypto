#pragma once

#include <chrono>

#include <flatbuffers/flatbuffers.h>
#include <zmq.hpp>
#include <krypto/network/helper.h>
#include <krypto/logger.h>

namespace krypto::network::rpc {
    template<typename Derived, typename ReceiveType, typename SendType>
    class WorkerBase {
    private:

        const zmq::context_t &context_;
        bool verbose_;
        std::string broker_;
        std::string service_;
        std::unique_ptr<zmq::socket_t> socket_;
        std::atomic_bool running_;
        void connect();
    protected:
        flatbuffers::FlatBufferBuilder fb_builder_;
    public:
        WorkerBase(
                zmq::context_t &context,
                std::string broker,
                std::string service,
                bool verbose);

        void send_ready();

        void send_disconnect();

        void start();

        void stop();
    };

    template<typename Derived, typename ReceiveType, typename SendType>
    WorkerBase<Derived, ReceiveType, SendType>::WorkerBase(
            zmq::context_t &context,
            std::string broker,
            std::string service,
            bool verbose) :
            context_{context}, broker_{std::move(broker)},
            service_{std::move(service)},
            verbose_{verbose},
            running_{false} {}

    template<typename Derived, typename ReceiveType, typename SendType>
    void WorkerBase<Derived, ReceiveType, SendType>::connect() {
        socket_ = std::make_unique<zmq::socket_t>(context_, ZMQ_DEALER);
        socket_->setsockopt(ZMQ_LINGER, nullptr, sizeof(int));
        auto identity = generate_id();
        socket_->setsockopt(ZMQ_IDENTITY, identity.c_str(), identity.size());
        socket_->connect(broker_.c_str());

        if (verbose_) {
            KRYP_LOG(info, "Connected to Broker @ {}", broker_);
        }

        send_ready();

        if (verbose_) {
            KRYP_LOG(info, "Send Ready Status to Broker @ {}", broker_);
        }
    }

    template<typename Derived, typename ReceiveType, typename SendType>
    void WorkerBase<Derived, ReceiveType, SendType>::send_ready() {
        send_empty_frame(*socket_, ZMQ_SNDMORE);
        send_status(*socket_, SocketStatus::READY, ZMQ_SNDMORE);
        send_string(*socket_, service_);
    }

    template<typename Derived, typename ReceiveType, typename SendType>
    void WorkerBase<Derived, ReceiveType, SendType>::start() {

        connect();

        while (running_) {
            zmq::pollitem_t poller = { *socket_,  0, ZMQ_POLLIN, 0 };
            if (poller.revents && ZMQ_POLLIN) {
                recv_empty_frame(*socket_);
                auto address = recv_string(*socket_);
                recv_empty_frame(*socket_);

                zmq::message_t payload_msg;
                socket_->recv(&payload_msg);

                auto &derived = static_cast<Derived &>(*this);
                auto payload = flatbuffers::GetRoot<ReceiveType>(payload_msg.data());

                derived.process(payload);

                zmq::message_t result_msg(fb_builder_.GetSize());
                std::memcpy(result_msg.data(), fb_builder_.GetBufferPointer(), fb_builder_.GetSize());

                send_empty_frame(*socket_, ZMQ_SNDMORE);
                send_status(*socket_, SocketStatus::REPLY, ZMQ_SNDMORE);
                send_string(*socket_, address, ZMQ_SNDMORE);
                send_empty_frame(*socket_, ZMQ_SNDMORE);
                socket_->send(result_msg);
            }
        }

        send_disconnect();
    }

    template<typename Derived, typename ReceiveType, typename SendType>
    void WorkerBase<Derived, ReceiveType, SendType>::stop() {
        running_ = false;
    }

    template<typename Derived, typename ReceiveType, typename SendType>
    void WorkerBase<Derived, ReceiveType, SendType>::send_disconnect() {
        send_empty_frame(*socket_, ZMQ_SNDMORE);
        send_status(*socket_, SocketStatus::DISCONNECT, ZMQ_SNDMORE);
        send_string(*socket_, service_);
    }
}