#include <krypto/pricing/client.h>

std::optional<krypto::pricing::Parser::receive_variant_t>
krypto::pricing::Parser::parse(const zmq::message_t &msg, krypto::utils::MsgType msg_type) {
    switch (msg_type) {
        case krypto::utils::MsgType::THEO: {
            return flatbuffers::GetRoot<krypto::serialization::TheoreticalSnapshot>(msg.data());
        }
        default: {
            break;
        }
    }
    return std::nullopt;
}

void krypto::pricing::PricingClient::register_listener(
        const krypto::pricing::PricingClient::theo_func_t &listener) {
    callbacks_.push_back(listener);
}

void krypto::pricing::PricingClient::unregister_listener(
        const krypto::pricing::PricingClient::theo_func_t &listener) {

}

void krypto::pricing::PricingClient::process(
        const krypto::serialization::TheoreticalSnapshot *snapshot) {
    std::for_each(std::begin(callbacks_), std::end(callbacks_), [&](auto &&c) {
        c(snapshot);
    });
}
