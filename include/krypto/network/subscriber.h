#pragma once

#include <iostream>
#include <utility>
#include <thread>
#include <atomic>

#include <zmq.hpp>

#include <krypto/serialization/serialization_generated.h>
#include <krypto/logger.h>


namespace krypto::network {

    template<typename Derived, typename MsgType, bool Sequenced = false>
    class SubscriberBase {
    private:
        zmq::socket_t sock_;
        const std::string endpoint_;
        bool connected_;
        const std::function<void(int64_t, MsgType)> callback_;
    public:
        SubscriberBase(zmq::context_t &, std::string, std::function<void(int64_t, MsgType)>);

        SubscriberBase(const SubscriberBase &other) = delete;

        SubscriberBase(SubscriberBase &&other) = delete;

        SubscriberBase &operator=(const SubscriberBase &other) = delete;

        SubscriberBase &operator=(SubscriberBase &&other) = delete;

        ~SubscriberBase();

        void connect();

        void disconnect();

        void subscribe(const std::string &topic);

        void recv();
    };

    template<typename Derived, typename MsgType, bool Sequenced>
    SubscriberBase<Derived, MsgType, Sequenced>::SubscriberBase(zmq::context_t &context, std::string endpoint,
                                                                      std::function<void(int64_t, MsgType)> f) :
            sock_(context, ZMQ_SUB), endpoint_(std::move(endpoint)), connected_(false),
            callback_(std::move(f)) {}

    template<typename Derived, typename MsgType, bool Sequenced>
    SubscriberBase<Derived, MsgType, Sequenced>::~SubscriberBase() {
        disconnect();
    }

    template<typename Derived, typename MsgType, bool Sequenced>
    void SubscriberBase<Derived, MsgType, Sequenced>::subscribe(const std::string &topic) {
        sock_.setsockopt(ZMQ_SUBSCRIBE, &topic[0], topic.length());
    }

    template<typename Derived, typename MsgType, bool Sequenced>
    void SubscriberBase<Derived, MsgType, Sequenced>::connect() {
        if (!connected_) {
            sock_.connect(endpoint_);
            connected_ = true;
        }
    }

    template<typename Derived, typename MsgType, bool Sequenced>
    void SubscriberBase<Derived, MsgType, Sequenced>::disconnect() {
        if (connected_) {
            connected_ = false;
            KRYP_LOG(info, "Disconnecting from {} ... ", endpoint_);
            sock_.disconnect(endpoint_);
            sock_.close();
            KRYP_LOG(info, "Disconnected!");
        }
    }

    template<typename Derived, typename MsgType, bool Sequenced>
    void SubscriberBase<Derived, MsgType, Sequenced>::recv() {

        zmq::message_t topic_msg;
        sock_.recv(&topic_msg);
        auto topic = std::string(static_cast<char *>(topic_msg.data()), topic_msg.size());

        int64_t seq_no = 0;

        if constexpr (Sequenced) {
            // Receive 2
            zmq::message_t sn_msg;
            sock_.recv(&sn_msg);

            auto sn = flatbuffers::GetRoot<krypto::serialization::SequenceNumber>(sn_msg.data());
            seq_no = sn->value();
        }

        zmq::message_t payload_msg;
        sock_.recv(&payload_msg);

        auto &derived = static_cast<Derived &>(*this);
        MsgType payload = derived.parse(payload_msg.data());

        callback_(seq_no, payload);
    }
};

