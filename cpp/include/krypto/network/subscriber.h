#pragma once

#include <iostream>
#include <utility>
#include <thread>
#include <atomic>
#include <variant>

#include <zmq.hpp>

#include <krypto/logger.h>
#include "krypto/utils/common.h"
#include <krypto/utils/types.h>


namespace krypto::network {

    template<typename Derived, typename Parser, bool Verbose = false>
    class Subscriber {
    private:
        std::unique_ptr<zmq::socket_t> socket_;
        std::string endpoint_;
        bool connected_;
        std::atomic_bool running_;
        std::unordered_map<std::string, krypto::utils::MsgType> msg_type_ref_;

        void connect();

        void disconnect();

        void recv();

    public:
        Subscriber(zmq::context_t &context, std::string);

        Subscriber(const Subscriber<Derived, Parser, Verbose> &other) = delete;

        Subscriber(Subscriber<Derived, Parser, Verbose> &&other) = delete;

        Subscriber<Derived, Parser, Verbose> &operator=(const Subscriber<Derived, Parser, Verbose> &) = delete;

        Subscriber<Derived, Parser, Verbose> &operator=(Subscriber<Derived, Parser, Verbose> &&) = delete;

        ~Subscriber();

        void subscribe(const std::string &);

        void subscribe(const std::vector<std::string> &);

        void subscribe(krypto::utils::MsgType);

        void start();

        void stop();

        Derived &derived_instance() { return static_cast<Derived &>(*this); }

        Derived const &derived_instance() const { return static_cast<Derived const &>(*this); }
    };

    template<typename Consumer, typename Parser, bool Verbose>
    Subscriber<Consumer, Parser, Verbose>::Subscriber(zmq::context_t &context, std::string endpoint) :
            socket_{std::make_unique<zmq::socket_t>(context, ZMQ_SUB)},
            endpoint_(std::move(endpoint)),
            connected_(false),
            running_{false},
            msg_type_ref_{krypto::utils::name_to_msg_type()} {

        connect();
    }

    template<typename Consumer, typename Parser, bool Verbose>
    Subscriber<Consumer, Parser, Verbose>::~Subscriber() {
        disconnect();
    }

    template<typename Consumer, typename Parser, bool Verbose>
    void Subscriber<Consumer, Parser, Verbose>::subscribe(const std::string &topic) {
        KRYP_LOG(info, "Subscribing to topic: {}", topic);
        socket_->set(zmq::sockopt::subscribe, topic);
    }

    template<typename Consumer, typename Parser, bool Verbose>
    void Subscriber<Consumer, Parser, Verbose>::subscribe(const std::vector<std::string> &topics) {
        std::for_each(topics.begin(), topics.end(), [this](auto &&topic) {
            subscribe(topic);
        });
    }

    template<typename Consumer, typename Parser, bool Verbose>
    void Subscriber<Consumer, Parser, Verbose>::subscribe(const krypto::utils::MsgType msg_type) {
        auto topic = krypto::utils::MsgTypeNames[static_cast<uint8_t>(msg_type)];
        subscribe(topic);
    }

    template<typename Consumer, typename Parser, bool Verbose>
    void Subscriber<Consumer, Parser, Verbose>::connect() {
        if (!connected_) {
            KRYP_LOG(info, "Connecting to {} ... ", endpoint_);
            socket_->connect(endpoint_);
            connected_ = true;
            KRYP_LOG(info, "Connected to {}", endpoint_);
        }
    }

    template<typename Consumer, typename Parser, bool Verbose>
    void Subscriber<Consumer, Parser, Verbose>::disconnect() {
        if (connected_) {
            connected_ = false;
            KRYP_LOG(info, "Disconnecting from {} ... ", endpoint_);
            socket_->disconnect(endpoint_);
            socket_->close();
            KRYP_LOG(info, "Disconnected!");
        }
    }


    template<typename Consumer, typename Parser, bool Verbose>
    void Subscriber<Consumer, Parser, Verbose>::recv() {

        zmq::message_t topic_msg;
        auto topic_size = socket_->recv(topic_msg);
        if (!topic_size.has_value()) {
            KRYP_LOG(error, "topic had 0 size");
            return;
        }
        auto topic = std::string(static_cast<char *>(topic_msg.data()), topic_msg.size());

        auto topic_prefix = topic.substr(0, 2);
        auto msg_type = msg_type_ref_.at(topic_prefix);

        zmq::message_t payload_msg;
        auto payload_size = socket_->recv(payload_msg, zmq::recv_flags::none);
        if (!payload_size.has_value()) {
            KRYP_LOG(error, "payload had 0 size");
            return;
        }
        auto payload = Parser::parse(payload_msg, msg_type);

        if (payload.has_value()) {
            std::visit([&](auto &&x) { derived_instance().process(x); }, payload.value());
        } else {
            KRYP_LOG(warn, "Unknown Topic Received: {}", topic);
        }
    }

    template<typename Consumer, typename Parser, bool Verbose>
    void Subscriber<Consumer, Parser, Verbose>::start() {
        KRYP_LOG(info, "Starting subscriber thread");
        running_ = true;
        while (running_) {
            recv();
        }
    }

    template<typename Consumer, typename Parser, bool Verbose>
    void Subscriber<Consumer, Parser, Verbose>::stop() {
        KRYP_LOG(info, "Stopping subscriber thread");
        running_ = false;
    }
}

