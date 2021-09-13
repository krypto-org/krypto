#pragma once

#include <variant>
#include <zmq.hpp>

#include <krypto/utils/message_types.h>
#include <krypto/serialization/serialization_generated.h>
#include <krypto/network/subscriber.h>

namespace krypto::pricing {
    struct Parser final {
        using receive_variant_t = std::variant<
                const krypto::serialization::TheoreticalSnapshot *>;

        static std::optional<receive_variant_t> parse(const zmq::message_t &msg,
                                                      krypto::utils::MsgType msg_type);
    };

    class PricingClient final : public krypto::network::Subscriber<PricingClient, Parser> {
    public:
        using theo_func_t = std::function<void(const krypto::serialization::TheoreticalSnapshot *)>;
        using krypto::network::Subscriber<PricingClient, Parser>::Subscriber;
    private:
        std::vector<theo_func_t> callbacks_;
    public:
        void register_listener(const theo_func_t &listener);

        void unregister_listener(const theo_func_t &listener);

        void process(const krypto::serialization::TheoreticalSnapshot *quote);
    };
}