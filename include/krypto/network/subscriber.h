#pragma once

#include <iostream>
#include <utility>
#include <thread>
#include <atomic>

#include <zmq.hpp>

#include <krypto/serialization/serialization_generated.h>
#include <krypto/logger.h>


namespace krypto::network {

    template<bool Verbose = false>
    class Subscriber {
    private:
        zmq::context_t context_;
        std::string endpoint_;
        bool connected_;
        std::unique_ptr<zmq::socket_t> socket_;

        void connect();

        void disconnect();

    public:
        explicit Subscriber(std::string);

        Subscriber(const Subscriber<Verbose> &other) = delete;

        Subscriber(Subscriber<Verbose> &&other) = delete;

        Subscriber<Verbose> &operator=(const Subscriber<Verbose> &) = delete;

        Subscriber<Verbose> &operator=(Subscriber<Verbose> &&) = delete;

        ~Subscriber();

        void subscribe(const std::string &);

        void subscribe(const std::vector<std::string> &);

        template<typename MsgType>
        const MsgType *recv();
    };

    template<bool Verbose>
    Subscriber<Verbose>::Subscriber(std::string endpoint) :
            context_{1},
            endpoint_(std::move(endpoint)),
            connected_(false) {
        socket_ = std::make_unique<zmq::socket_t>(context_, ZMQ_SUB);
        connect();
    }

    template<bool Verbose>
    Subscriber<Verbose>::~Subscriber() {
        disconnect();
    }

    template<bool Verbose>
    void Subscriber<Verbose>::subscribe(const std::string &topic) {
        socket_->setsockopt(ZMQ_SUBSCRIBE, &topic[0], topic.length());
    }

    template<bool Verbose>
    void Subscriber<Verbose>::subscribe(const std::vector<std::string> &topics) {
        std::for_each(topics.begin(), topics.end(), [this](auto &&topic) {
            subscribe(topic);
        });
    }

    template<bool Verbose>
    void Subscriber<Verbose>::connect() {
        if (!connected_) {
            socket_->connect(endpoint_);
            connected_ = true;
        }
    }

    template<bool Verbose>
    void Subscriber<Verbose>::disconnect() {
        if (connected_) {
            connected_ = false;
            KRYP_LOG(info, "Disconnecting from {} ... ", endpoint_);
            socket_->disconnect(endpoint_);
            socket_->close();
            KRYP_LOG(info, "Disconnected!");
        }
    }

    template<bool Verbose>
    template<typename MsgType>
    const MsgType *Subscriber<Verbose>::recv() {

        zmq::message_t topic_msg;
        socket_->recv(&topic_msg);
        auto topic = std::string(static_cast<char *>(topic_msg.data()), topic_msg.size());

        zmq::message_t payload_msg;
        socket_->recv(&payload_msg);

        if constexpr (Verbose) {
            KRYP_LOG(info, "Received data -- topic: {} | payload size: {}", topic, payload_msg.size());
        }

        auto payload = flatbuffers::GetRoot<MsgType>(payload_msg.data());

        return payload;
    }
}

