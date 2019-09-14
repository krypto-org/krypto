#pragma once

#include <iostream>
#include <utility>
#include <thread>
#include <atomic>
#include <variant>

#include <zmq.hpp>

#include <krypto/logger.h>
#include "krypto/utils/common.h"


namespace krypto::network {

    template<typename Derived, typename Parser, bool Verbose = false>
    class Subscriber {
    private:
        using receive_variant_t = std::variant<
                const krypto::serialization::Quote *,
                const krypto::serialization::Trade *,
                const krypto::serialization::Heartbeat *>;

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
    Subscriber<Consumer, Parser, Verbose>::Subscriber(std::string endpoint) :
            context_{1},
            endpoint_(std::move(endpoint)),
            connected_(false),
            running_{false} {
        socket_ = std::make_unique<zmq::socket_t>(context_, ZMQ_SUB);
        connect();
    }

    template<typename Consumer, typename Parser, bool Verbose>
    Subscriber<Consumer, Parser, Verbose>::~Subscriber() {
        disconnect();
    }

    template<typename Consumer, typename Parser, bool Verbose>
    void Subscriber<Consumer, Parser, Verbose>::subscribe(const std::string &topic) {
        socket_->setsockopt(ZMQ_SUBSCRIBE, &topic[0], topic.length());
    }

    template<typename Consumer, typename Parser, bool Verbose>
    void Subscriber<Consumer, Parser, Verbose>::subscribe(const std::vector<std::string> &topics) {
        std::for_each(topics.begin(), topics.end(), [this](auto &&topic) {
            subscribe(topic);
        });
    }

    template<typename Consumer, typename Parser, bool Verbose>
    void Subscriber<Consumer, Parser, Verbose>::subscribe(const krypto::utils::MsgType msg_type) {
        auto topic = krypto::utils::MsgTypeEnum::enum_to_names.at(msg_type);
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
        socket_->recv(&topic_msg);
        auto topic = std::string(static_cast<char *>(topic_msg.data()), topic_msg.size());

        auto topic_prefix = topic.substr(0, 1);
        auto msg_type = krypto::utils::MsgTypeEnum::names_to_enums.at(topic_prefix);

        zmq::message_t payload_msg;
        socket_->recv(&payload_msg);

        if constexpr (Verbose) {
            KRYP_LOG(info, "Received data -- topic: {} | payload size: {}", topic, payload_msg.size());
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
        running_ = true;
        while (running_) {
            recv();
        }
    }

    template<typename Consumer, typename Parser, bool Verbose>
    void Subscriber<Consumer, Parser, Verbose>::stop() {
        running_ = false;
    }
}

