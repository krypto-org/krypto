#pragma once

#include <iostream>
#include <utility>
#include <thread>
#include <atomic>

#include <zmq.hpp>

#include <krypto/serialization/serialization_generated.h>
#include <krypto/logger.h>
#include "krypto/utils/common.h"


namespace krypto::network {

    template<typename Consumer, bool Verbose = false>
    class Subscriber : public Consumer {
    private:
        zmq::context_t context_;
        std::string endpoint_;
        bool connected_;
        std::atomic_bool running_;
        std::unique_ptr<zmq::socket_t> socket_;

        void connect();

        void disconnect();

        void recv();

    public:
        explicit Subscriber(std::string);

        Subscriber(const Subscriber<Consumer, Verbose> &other) = delete;

        Subscriber(Subscriber<Consumer, Verbose> &&other) = delete;

        Subscriber<Consumer, Verbose> &operator=(const Subscriber<Consumer, Verbose> &) = delete;

        Subscriber<Consumer, Verbose> &operator=(Subscriber<Consumer, Verbose> &&) = delete;

        ~Subscriber();

        void subscribe(const std::string &);

        void subscribe(const std::vector<std::string> &);

        void subscribe(krypto::utils::MsgType );

        void start();

        void stop();
    };

    template<typename Consumer, bool Verbose>
    Subscriber<Consumer, Verbose>::Subscriber(std::string endpoint) :
            context_{1},
            endpoint_(std::move(endpoint)),
            connected_(false),
            running_{false} {
        socket_ = std::make_unique<zmq::socket_t>(context_, ZMQ_SUB);
        connect();
    }

    template<typename Consumer, bool Verbose>
    Subscriber<Consumer, Verbose>::~Subscriber() {
        disconnect();
    }

    template<typename Consumer, bool Verbose>
    void Subscriber<Consumer, Verbose>::subscribe(const std::string &topic) {
        socket_->setsockopt(ZMQ_SUBSCRIBE, &topic[0], topic.length());
    }

    template<typename Consumer, bool Verbose>
    void Subscriber<Consumer, Verbose>::subscribe(const std::vector<std::string> &topics) {
        std::for_each(topics.begin(), topics.end(), [this](auto &&topic) {
            subscribe(topic);
        });
    }

    template<typename Consumer, bool Verbose>
    void Subscriber<Consumer, Verbose>::subscribe(const krypto::utils::MsgType msg_type) {
        auto topic = krypto::utils::MsgTypeEnum::enum_to_names.at(msg_type);
        subscribe(topic);
    }

    template<typename Consumer, bool Verbose>
    void Subscriber<Consumer, Verbose>::connect() {
        if (!connected_) {
            KRYP_LOG(info, "Connecting to {} ... ", endpoint_);
            socket_->connect(endpoint_);
            connected_ = true;
            KRYP_LOG(info, "Connected to {}", endpoint_);
        }
    }

    template<typename Consumer, bool Verbose>
    void Subscriber<Consumer, Verbose>::disconnect() {
        if (connected_) {
            connected_ = false;
            KRYP_LOG(info, "Disconnecting from {} ... ", endpoint_);
            socket_->disconnect(endpoint_);
            socket_->close();
            KRYP_LOG(info, "Disconnected!");
        }
    }


    template<typename Consumer, bool Verbose>
    void Subscriber<Consumer, Verbose>::recv() {

        zmq::message_t topic_msg;
        socket_->recv(&topic_msg);
        auto topic = std::string(static_cast<char *>(topic_msg.data()), topic_msg.size());

        auto topic_prefix = topic.substr(0, 1);
        auto msg_type = krypto::utils::MsgTypeEnum::names_to_enums.at(topic_prefix);

        zmq::message_t payload_msg;
        socket_->recv(&payload_msg);

        if constexpr (Verbose) {
            KRYP_LOG(info, "Received data -- topic: {} | payload size: {}", topic, payload_msg.size());
        }

        switch (msg_type) {
            case krypto::utils::MsgType::QUOTE: {
                auto payload = flatbuffers::GetRoot<krypto::serialization::Quote>(payload_msg.data());
                Consumer::consume(payload);
                return;
            }
            case krypto::utils::MsgType::TRADE: {
                auto payload = flatbuffers::GetRoot<krypto::serialization::Trade>(payload_msg.data());
                Consumer::consume(payload);
                return;
            }
            default: {
                KRYP_LOG(warn, "Unknown Topic Received: {}", topic);
            }
        }
    }

    template<typename Consumer, bool Verbose>
    void Subscriber<Consumer, Verbose>::start() {
        running_ = true;
        while (running_) {
            recv();
        }
    }

    template<typename Consumer, bool Verbose>
    void Subscriber<Consumer, Verbose>::stop() {
        running_ = false;
    }


}

