#pragma once

#include <bitset>

#include <flatbuffers/flatbuffers.h>
#include <zmq.hpp>

#include <krypto/serialization/serialization_generated.h>
#include <krypto/types.h>
#include <krypto/logger.h>
#include <krypto/config.h>

namespace krypto::network {

    template<typename Derived>
    class PublisherBase {
    private:
        std::unique_ptr<zmq::socket_t> socket_;
        const std::string endpoint_;
        bool connected_;
    protected:
        flatbuffers::FlatBufferBuilder fb_builder_;
    public:
        explicit PublisherBase(zmq::context_t& context, std::string endpoint);

        PublisherBase(const PublisherBase &other) = delete;

        PublisherBase(PublisherBase &&other) = delete;

        PublisherBase &operator=(const PublisherBase &other) = delete;

        PublisherBase &operator=(PublisherBase &&other) = delete;

        ~PublisherBase();

        void connect();

        template<typename... Args>
        bool send(const std::string &topic, Args... args);
    };

    template<typename Derived>
    PublisherBase<Derived>::PublisherBase(zmq::context_t& context, std::string endpoint) :
            socket_{std::make_unique<zmq::socket_t>(context, ZMQ_PUB)},
            endpoint_(std::move(endpoint)),
            connected_(false) {
    }

    template<typename Derived>
    PublisherBase<Derived>::~PublisherBase() {
        connected_ = false;
        socket_->close();
    }

    template<typename Derived>
    void PublisherBase<Derived>::connect() {
        KRYP_LOG(info, "Connecting to proxy @ {}", endpoint_);
        socket_->connect(endpoint_);
        connected_ = true;
    }

    template<typename Derived>
    template<typename... Args>
    bool PublisherBase<Derived>::send(const std::string &topic, Args... args) {
        std::bitset<2> status;

        zmq::message_t topic_nsg(topic.size());
        std::memcpy(topic_nsg.data(), topic.data(), topic.size());

        status.set(0, socket_->send(topic_nsg, ZMQ_SNDMORE));

        auto &derived = static_cast<Derived &>(*this);
        derived.serialize(args...);

        zmq::message_t payload_msg(fb_builder_.GetSize());
        std::memcpy(payload_msg.data(), fb_builder_.GetBufferPointer(), fb_builder_.GetSize());

        status.set(1, socket_->send(payload_msg));

        return status.all();
    }
}
