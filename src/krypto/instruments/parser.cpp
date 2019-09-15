#include <krypto/instruments/parser.h>

std::optional<krypto::instruments::Parser::receive_variant_t>
krypto::instruments::Parser::parse(
        const zmq::message_t &msg, krypto::utils::MsgType msg_type) {
    if (msg_type == krypto::utils::MsgType::INSTRUMENT_RESPONSE) {
        return flatbuffers::GetRoot<krypto::serialization::InstrumentResponse>(msg.data());
    }

    return std::nullopt;
}
