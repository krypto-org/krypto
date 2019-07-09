#pragma once

#include <utility>
#include <bitset>
#include <atomic>

#include <flatbuffers/flatbuffers.h>
#include <zmq.hpp>

#include <krypto/network/majordomo/mdp.h>
#include <krypto/logger.h>

namespace krypto::network::mdp {

    template<typename Derived>
    class MdpClientBase {
    private:
        const zmq::context_t &context_;
        uint32_t timeout_;
        uint16_t retries_;
        bool verbose_;
        std::string broker_;
        std::atomic_bool connected_;

        std::unique_ptr<zmq::socket_t> socket_;

        void connect();

        template<typename... Args>
        const bool send_impl(const std::string &, Args...);

        template<typename ReceiveType>
        const std::optional<ReceiveType> receive_impl(const std::string& );

    protected:
        flatbuffers::FlatBufferBuilder fb_builder_;
    public:
        MdpClientBase(
                zmq::context_t &context,
                uint32_t timeout,
                uint16_t retries,
                bool verbose,
                std::string broker);

        ~MdpClientBase();

        const uint32_t timeout() const;

        const uint16_t retries() const;

        const bool verbose() const;

        const std::string &broker() const;

        template<typename ReceiveType, typename... Args>
        const std::optional<ReceiveType> send(std::string service, Args... args);
    };

    template<typename Derived>
    MdpClientBase<Derived>::MdpClientBase(
            zmq::context_t &context,
            uint32_t timeout,
            uint16_t retries,
            bool verbose,
            std::string broker) :
            context_{context},
            timeout_{timeout},
            retries_{retries},
            verbose_{verbose},
            broker_{std::move(broker)},
            connected_{false} {
    }

    template<typename Derived>
    const uint32_t MdpClientBase<Derived>::timeout() const {
        return timeout_;
    }

    template<typename Derived>
    const uint16_t MdpClientBase<Derived>::retries() const {
        return retries_;
    }

    template<typename Derived>
    const bool MdpClientBase<Derived>::verbose() const {
        return verbose_;
    }

    template<typename Derived>
    const std::string &MdpClientBase<Derived>::broker() const {
        return broker_;
    }

    template<typename Derived>
    void MdpClientBase<Derived>::connect() {
        socket_ = std::make_unique<zmq::socket_t>(context_, ZMQ_REQ);
        socket_->setsockopt(ZMQ_LINGER, nullptr, sizeof(int));
        auto identity = generate_id();
        socket_->setsockopt(ZMQ_IDENTITY, identity.c_str(), identity.size());
        socket_->connect (broker_.c_str());

        if (verbose_) {
            KRYP_LOG(info, "Connecting to Broker @ {}", broker_);
        }
    }

    template<typename Derived>
    template<typename ReceiveType, typename... Args>
    const std::optional<ReceiveType> MdpClientBase<Derived>::send(std::string service_name, Args... args) {
        int retries_left = retries_;
        send_impl(service_name, args...);

        while (connected_ && retries_left) {
            zmq::pollitem_t items[] = {{socket_, 0, ZMQ_POLLIN, 0}};
            zmq::poll (items, 1, timeout_);

            if (verbose_) {
                KRYP_LOG(debug, "Sending message to {}", service_name);
            }

            if (items [0].revents & ZMQ_POLLIN) {

                auto received = receive_impl(service_name);

                if (received.has_value()) {
                    return std::move(received); // Success
                }
            }
            else {
                if (--retries_left) {
                    if (verbose_) {
                        KRYP_LOG(debug, "No reply reconnecting ...");
                    }
                    connect();
                    send_impl(service_name);
                }
                else {
                    if (verbose_) {
                        KRYP_LOG(debug, "Permanent error, abandoning request");
                    }
                    break; //  Give up
                }
            }
        }

        return std::nullopt;
    }

    template<typename Derived>
    MdpClientBase<Derived>::~MdpClientBase() {
        connected_ = false;
        socket_->close();
    }

    template<typename Derived>
    template<typename... Args>
    const bool MdpClientBase<Derived>::send_impl(const std::string &service_name, Args... args) {

        // 1. Send client version
        // 2. Send Service Name
        // 3. Send binary data

        std::bitset<3> status;

        zmq::message_t version_msg(MDP_CLIENT.size());
        std::memcpy(version_msg.data(), MDP_CLIENT.data(), MDP_CLIENT.size());
        status.set(0, socket_->send(version_msg, ZMQ_SNDMORE));

        zmq::message_t service_name_msg(MDP_CLIENT.size());
        std::memcpy(service_name_msg.data(), service_name.data(), service_name.size());
        status.set(1, socket_->send(service_name_msg, ZMQ_SNDMORE));

        auto &derived = static_cast<Derived &>(*this);
        derived.serialize(args...);

        zmq::message_t payload_msg(fb_builder_.GetSize());
        std::memcpy(payload_msg.data(), fb_builder_.GetBufferPointer(), fb_builder_.GetSize());

        status.set(2, socket_->send(payload_msg));

        return status.all();
    }

    template<typename Derived>
    template<typename ReceiveType>
    const std::optional<ReceiveType>  MdpClientBase<Derived>::receive_impl(const std::string& service_name) {

        zmq::message_t version_msg;
        if (!socket_->recv(&version_msg, 0)) {
            return std::nullopt;
        }

        auto version = std::string(static_cast<char *>(version_msg.data()), version_msg.size());

        if (version != MDP_CLIENT) {
            KRYP_LOG(warn, "Version --- IN {} != OUT {}",
                    version, MDP_CLIENT);
            return std::nullopt;
        }

        zmq::message_t service_msg;

        if (!socket_->recv(&service_msg, 0)) {
            return std::nullopt;
        }

        auto s_name = std::string(static_cast<char *>(version_msg.data()), version_msg.size());

        if (s_name != service_name) {
            KRYP_LOG(warn, "Service Name --- IN {} != OUT {}", s_name, service_name);
            return std::nullopt;
        }

        zmq::message_t payload_msg;
        socket_->recv(&payload_msg);

        auto &derived = static_cast<Derived &>(*this);
        ReceiveType payload = derived.deserialize(payload_msg.data());

        if (payload_msg.more()) {
            KRYP_LOG(error, "Received more than 3 frames");
        }

        return std::optional{payload};
    }
}