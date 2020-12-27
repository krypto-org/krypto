#pragma once

#include <utility>
#include <bitset>
#include <memory>
#include <variant>

#include <flatbuffers/flatbuffers.h>
#include <zmq.hpp>

#include <krypto/network/helper.h>
#include <krypto/utils/common.h>
#include <krypto/logger.h>
#include <krypto/config.h>
#include <krypto/utils/types.h>

namespace krypto::network::rpc {

    template<typename Derived, typename RequestVariant, typename Parser, bool Verbose = false>
    class ClientBase {
    private:
        std::unique_ptr<zmq::socket_t> socket_;
        std::string broker_;

        void send_impl(const std::string &, const RequestVariant &);

    protected:
        flatbuffers::FlatBufferBuilder fb_builder_;
    public:
        ClientBase(zmq::context_t &context, const krypto::Config &config);

        void connect();

        void send(const std::string &service, const RequestVariant &);

        bool receive(int timeout = -1);
    };

    template<typename Derived, typename RequestVariant, typename Parser, bool Verbose>
    ClientBase<Derived, RequestVariant, Parser, Verbose>::ClientBase(
            zmq::context_t &context,
            const krypto::Config &config) :
            socket_{std::make_unique<zmq::socket_t>(context, ZMQ_DEALER)},
            broker_{config.at<std::string>("/services/rpc/broker/frontend/client")} {
        connect();
    }

    template<typename Derived, typename RequestVariant, typename Parser, bool Verbose>
    void ClientBase<Derived, RequestVariant, Parser, Verbose>::connect() {
        if constexpr  (Verbose) {
            KRYP_LOG(info, "Connecting to broker @ {}", broker_);
        }

        int linger = 0;
        socket_->setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
        auto identity = "client-" + generate_id();
        socket_->setsockopt(ZMQ_IDENTITY, identity.c_str(), identity.size());
        socket_->connect(broker_.c_str());
    }

    template<typename Derived, typename RequestVariant, typename Parser, bool Verbose>
    void ClientBase<Derived, RequestVariant, Parser, Verbose>::send(const std::string &service_name,
                                                                    const RequestVariant &request) {
        send_impl(service_name, request);
        if constexpr  (Verbose) {
            KRYP_LOG(debug, "Sent message to {}", service_name);
        }
    }

    template<typename Derived, typename RequestVariant, typename Parser, bool Verbose>
    void ClientBase<Derived, RequestVariant, Parser, Verbose>::send_impl(
            const std::string &service_name, const RequestVariant &request) {
        send_empty_frame(*socket_, zmq::send_flags::sndmore);
        send_string(*socket_, service_name, zmq::send_flags::sndmore);

        auto msg_type = std::visit(
                [&](auto &&r) -> krypto::utils::MsgType { return std::remove_reference<decltype(r)>::type::message_type; },
                request);

        send_msg_type(*socket_, msg_type, zmq::send_flags::sndmore);

        auto &derived = static_cast<Derived &>(*this);
        std::visit([&](auto &&r) { derived.serialize(r); }, request);

        zmq::message_t payload_msg(fb_builder_.GetSize());
        std::memcpy(payload_msg.data(), fb_builder_.GetBufferPointer(), fb_builder_.GetSize());

        socket_->send(payload_msg, zmq::send_flags::none);
    }

    template<typename Derived, typename RequestVariant, typename Parser, bool Verbose>
    bool ClientBase<Derived, RequestVariant, Parser, Verbose>::receive(int timeout) {

        zmq::pollitem_t items[] = {
                {*socket_, 0, ZMQ_POLLIN, 0},
        };

        zmq::poll(&items[0], 1, timeout);

        if (items[0].revents && ZMQ_POLLIN) {

            recv_empty_frame(*socket_);
            auto s_name = recv_string(*socket_);
            auto msg_type = recv_msg_type(*socket_);

            if (msg_type == krypto::utils::MsgType::UNDEFINED) {
                KRYP_LOG(error, "Received undefined message");
                return false;
            }

            if (msg_type == krypto::utils::MsgType::NO_PAYLOAD) {
                KRYP_LOG(info, "Received acknowledgement message - no payload");
                return true;
            }

            zmq::message_t payload_msg;
            socket_->recv(&payload_msg);

            if (payload_msg.more()) {
                KRYP_LOG(error, "Received more than 3 frames");
            }

            auto &derived = static_cast<Derived &>(*this);

            auto payload = Parser::parse(payload_msg, msg_type);

            if (payload.has_value()) {
                std::visit([&](auto &&x) { derived.process(x); }, payload.value());
            } else {
                KRYP_LOG(warn, "Unknown Response Type Received");
            }

            return true;
        }

        return false;
    }

}