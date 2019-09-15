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

namespace krypto::network::rpc {

    template<typename Derived, typename RequestVariant, typename Parser, bool Verbose = false>
    class ClientBase {
    private:
        zmq::context_t context_;
        std::string broker_;
        std::unique_ptr<zmq::socket_t> socket_;

        bool send_impl(const std::string &, const RequestVariant &);

        void receive_impl(const std::string &);

    protected:
        flatbuffers::FlatBufferBuilder fb_builder_;
    public:
        explicit ClientBase(const krypto::Config &config);

        void connect();

        void send(std::string service, const RequestVariant &);
    };

    template<typename Derived, typename RequestVariant, typename Parser,bool Verbose>
    ClientBase<Derived, RequestVariant, Parser,Verbose>::ClientBase(
            const krypto::Config &config) :
            context_{1},
            broker_{config.at<std::string>("/services/rpc/broker/frontend/client")} {

        socket_ = std::make_unique<zmq::socket_t>(context_, ZMQ_DEALER);

        connect();
    }

    template<typename Derived, typename RequestVariant, typename Parser,bool Verbose>
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

    template<typename Derived, typename RequestVariant,typename Parser, bool Verbose>
    void ClientBase<Derived, RequestVariant, Parser, Verbose>::send(std::string service_name, const RequestVariant &request) {

        send_impl(service_name, request);

        if constexpr  (Verbose) {
            KRYP_LOG(debug, "Sent message to {}", service_name);
        }

        receive_impl(service_name);
    }

    template<typename Derived, typename RequestVariant, typename Parser,bool Verbose>
    bool ClientBase<Derived, RequestVariant, Parser, Verbose>::send_impl(
            const std::string &service_name, const RequestVariant &request) {
        std::bitset<2> status;

        send_empty_frame(*socket_, ZMQ_SNDMORE);

        zmq::message_t service_name_msg(service_name.size());
        std::memcpy(service_name_msg.data(), service_name.data(), service_name.size());
        status.set(0, socket_->send(service_name_msg, ZMQ_SNDMORE));

        auto &derived = static_cast<Derived &>(*this);

        std::visit([&](auto &&r) { derived.serialize(r); }, request);

        zmq::message_t payload_msg(fb_builder_.GetSize());
        std::memcpy(payload_msg.data(), fb_builder_.GetBufferPointer(), fb_builder_.GetSize());

        status.set(1, socket_->send(payload_msg));

        return status.all();
    }

    template<typename Derived, typename RequestVariant, typename Parser,bool Verbose>
    void ClientBase<Derived, RequestVariant,Parser, Verbose>::receive_impl(const std::string &service_name) {

        recv_empty_frame(*socket_);

        zmq::message_t service_msg;

        if (!socket_->recv(&service_msg, 0)) {
            return;
        }

        auto s_name = std::string(static_cast<char *>(service_msg.data()), service_msg.size());

        if (s_name != service_name) {
            KRYP_LOG(warn, "Service name received [{}] != expected [{}]", s_name, service_name);
            return;
        }

        if (!service_msg.more()) {
            KRYP_LOG(error, "No data received");
            return;
        }

        zmq::message_t payload_msg;
        socket_->recv(&payload_msg);

        if (payload_msg.more()) {
            KRYP_LOG(error, "Received more than 2 frames");
        }

        auto &derived = static_cast<Derived &>(*this);

        auto payload = Parser::parse(payload_msg, krypto::utils::MsgType::INSTRUMENT_RESPONSE);

        if (payload.has_value()) {
            std::visit([&](auto &&x) { derived.process(x); }, payload.value());
        } else {
            KRYP_LOG(warn, "Unknown Response Type Received");
        }
    }
}