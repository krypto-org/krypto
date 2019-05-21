#pragma once

#include <bitset>

#include <flatbuffers/flatbuffers.h>
#include <zmq.hpp>

#include <krypto/serialization/serialization_generated.h>
#include <krypto/types.h>

namespace krypto::network {

    template<typename Derived, typename... Args>
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

        bool send(const std::string &topic, Args... args);

        bool send(const std::string &topic, int64_t sequence, Args... args);
    };

    template<typename Derived, typename... Args>
    PublisherBase<Derived, Args...>::PublisherBase(zmq::context_t &context,
                                                         std::string endpoint) : context_(context),
                                                                                 sock_(context, ZMQ_PUB),
                                                                                 endpoint_(std::move(endpoint)),
                                                                                 connected_(false),
                                                                                 fb_builder_(1024) {
    }

    template<typename Derived, typename... Args>
    PublisherBase<Derived, Args...>::~PublisherBase() {
        connected_ = false;
        sock_.close();
    }

    template<typename Derived, typename... Args>
    template<bool WithProxy>
    void PublisherBase<Derived, Args...>::start() {
        if constexpr (WithProxy) {
            sock_.connect(endpoint_);
        } else {
            sock_.bind(endpoint_);
        }

        connected_ = true;
    }

    template<typename Derived, typename... Args>
    bool PublisherBase<Derived, Args...>::send(const std::string &topic, Args... args) {
        std::bitset<2> status;

        zmq::message_t topic_nsg(topic.size());
        std::memcpy(topic_nsg.data(), topic.data(), topic.size());

        status.set(0, sock_.send(topic_nsg, ZMQ_SNDMORE));

        zmq::message_t payload_msg;
        auto &derived = static_cast<Derived &>(*this);
        derived.generate_buffer(args...);
        std::memcpy(payload_msg.data(), fb_builder_.GetBufferPointer(), fb_builder_.GetSize());

        status.set(1, sock_.send(payload_msg));

        return status.all();
    }

    template<typename Derived, typename... Args>
    bool
    PublisherBase<Derived, Args...>::send(const std::string &topic, const krypto::SequenceNumber sequence,
                                             Args... args) {
        std::bitset<3> status;

        // Send Topic
        zmq::message_t topic_nsg(topic.size());
        std::memcpy(topic_nsg.data(), topic.data(), topic.size());
        status.set(0, sock_.send(topic_nsg, ZMQ_SNDMORE));

        // Create Sequence Message
        fb_builder_.Clear();
        krypto::serialization::SequenceNumberBuilder sn_builder(fb_builder_);
        sn_builder.add_value(sequence);
        auto sn = sn_builder.Finish();
        fb_builder_.Finish(sn);

        // Send Sequence Message
        zmq::message_t sn_msg(fb_builder_.GetBufferPointer(), fb_builder_.GetSize());
        status.set(1, sock_.send(sn_msg, ZMQ_SNDMORE));

        // Send Sequence Message

        fb_builder_.Clear();
        auto &derived = static_cast<Derived &>(*this);
        derived.generate_buffer(args...);
        zmq::message_t payload_msg(fb_builder_.GetBufferPointer(), fb_builder_.GetSize());
        status.set(2, sock_.send(payload_msg));

        return status.all();
    }
}
