#pragma once

#include <bitset>

#include <flatbuffers/flatbuffers.h>
#include <zmq.hpp>

#include <krypto/serialization/serialization_generated.h>
#include <krypto/types.h>
#include <krypto/logger.h>

namespace krypto::network {

    template<typename Derived>
    class PublisherBase {
    private:
        const zmq::context_t &context_;
        zmq::socket_t sock_;
        const std::string endpoint_;
        bool connected_;
    protected:
        flatbuffers::FlatBufferBuilder fb_builder_;
    public:
        PublisherBase(zmq::context_t &, std::string);

        PublisherBase(const PublisherBase &other) = delete;

        PublisherBase(PublisherBase &&other) = delete;

        PublisherBase &operator=(const PublisherBase &other) = delete;

        PublisherBase &operator=(PublisherBase &&other) = delete;

        ~PublisherBase();

        template<bool WithProxy>
        void start();

        template<typename... Args>
        bool send(const std::string &topic, Args... args);
    };

    template<typename Derived>
    PublisherBase<Derived>::PublisherBase(zmq::context_t &context,
                                          std::string endpoint) : context_(context),
                                                                  sock_(context, ZMQ_PUB),
                                                                  endpoint_(std::move(endpoint)),
                                                                  connected_(false),
                                                                  fb_builder_(1024) {
    }

    template<typename Derived>
    PublisherBase<Derived>::~PublisherBase() {
        connected_ = false;
        sock_.close();
    }

    template<typename Derived>
    template<bool WithProxy>
    void PublisherBase<Derived>::start() {
        if constexpr (WithProxy) {
            sock_.connect(endpoint_);
        } else {
            sock_.bind(endpoint_);
        }

        connected_ = true;
    }

    template<typename Derived>
    template<typename... Args>
    bool PublisherBase<Derived>::send(const std::string &topic, Args... args) {
        std::bitset<2> status;

        zmq::message_t topic_nsg(topic.size());
        std::memcpy(topic_nsg.data(), topic.data(), topic.size());

        status.set(0, sock_.send(topic_nsg, ZMQ_SNDMORE));

        auto &derived = static_cast<Derived &>(*this);
        derived.serialize(args...);

        zmq::message_t payload_msg(fb_builder_.GetSize());
        std::memcpy(payload_msg.data(), fb_builder_.GetBufferPointer(), fb_builder_.GetSize());

        status.set(1, sock_.send(payload_msg));

        return status.all();
    }
}
