#pragma once

#include <utility>
#include <bitset>
#include <atomic>

#include <flatbuffers/flatbuffers.h>
#include <zmq.hpp>

#include <krypto/network/helper.h>
#include <krypto/logger.h>

namespace krypto::network::rpc {

    template<typename Derived>
    class ClientBase {
    private:
        const zmq::context_t &context_;
        bool verbose_;
        std::string broker_;
        std::atomic_bool connected_;

        zmq::socket_t socket_;

        template<typename... Args>
        const bool send_impl(const std::string &, Args...);

        template<typename ReceiveType>
        const ReceiveType * receive_impl(const std::string &);

    protected:
        flatbuffers::FlatBufferBuilder fb_builder_;
    public:
        ClientBase(
                zmq::context_t &context,
                bool verbose,
                std::string broker);

        ~ClientBase();

        void connect();

        const bool verbose() const;

        const std::string &broker() const;

        template<typename ReceiveType, typename... Args>
        const ReceiveType * send(std::string service, Args... args);
    };

    template<typename Derived>
    ClientBase<Derived>::ClientBase(
            zmq::context_t &context,
            bool verbose,
            std::string broker) :
            context_{context},
            verbose_{verbose},
            broker_{std::move(broker)},
            connected_{false},
            socket_{context, ZMQ_REQ} {
        connect();
    }

    template<typename Derived>
    const bool ClientBase<Derived>::verbose() const {
        return verbose_;
    }

    template<typename Derived>
    const std::string &ClientBase<Derived>::broker() const {
        return broker_;
    }

    template<typename Derived>
    void ClientBase<Derived>::connect() {
        if (verbose_) {
            KRYP_LOG(info, "Connecting to Broker @ {}", broker_);
        }

        int linger = 0;
        socket_.setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
        auto identity = generate_id();
        socket_.setsockopt(ZMQ_IDENTITY, identity.c_str(), identity.size());
        socket_.connect(broker_.c_str());
    }

    template<typename Derived>
    template<typename ReceiveType, typename... Args>
    const ReceiveType * ClientBase<Derived>::send(std::string service_name, Args... args) {

        send_impl(service_name, args...);

        if (verbose_) {
            KRYP_LOG(debug, "Sent message to {}", service_name);
        }

        return receive_impl<ReceiveType>(service_name);
    }

    template<typename Derived>
    ClientBase<Derived>::~ClientBase() {
        connected_ = false;
        socket_.close();
    }

    template<typename Derived>
    template<typename... Args>
    const bool ClientBase<Derived>::send_impl(const std::string &service_name, Args... args) {

        // 2. Send Service Name
        // 3. Send binary data

        std::bitset<2> status;

        zmq::message_t service_name_msg(service_name.size());
        std::memcpy(service_name_msg.data(), service_name.data(), service_name.size());
        status.set(0, socket_.send(service_name_msg, ZMQ_SNDMORE));

        auto &derived = static_cast<Derived &>(*this);
        derived.serialize(args...);

        zmq::message_t payload_msg(fb_builder_.GetSize());
        std::memcpy(payload_msg.data(), fb_builder_.GetBufferPointer(), fb_builder_.GetSize());

        status.set(1, socket_.send(payload_msg));

        return status.all();
    }

    template<typename Derived>
    template<typename ReceiveType>
    const ReceiveType * ClientBase<Derived>::receive_impl(const std::string &service_name) {

        zmq::message_t service_msg;

        if (!socket_.recv(&service_msg, 0)) {
            return nullptr;
        }

        auto s_name = std::string(static_cast<char *>(service_msg.data()), service_msg.size());

        if (s_name != service_name) {
            KRYP_LOG(warn, "Service Name --- IN {} != OUT {}", s_name, service_name);
            return std::nullptr_t {};
        }

        if (!service_msg.more()) {
            KRYP_LOG(error, "No Data Received");
            return nullptr;
        }

        zmq::message_t payload_msg;
        socket_.recv(&payload_msg);

        auto &derived = static_cast<Derived &>(*this);
        auto payload = flatbuffers::GetRoot<ReceiveType>(payload_msg.data());

        if (payload_msg.more()) {
            KRYP_LOG(error, "Received more than 2 frames");
        }

        return payload;
    }
}