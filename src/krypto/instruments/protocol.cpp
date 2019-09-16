#include <krypto/instruments/protocol.h>

std::optional<krypto::instruments::ClientParser::receive_variant_t>
krypto::instruments::ClientParser::parse(
        const zmq::message_t &msg, krypto::utils::MsgType msg_type) {
    if (msg_type == krypto::utils::MsgType::INSTRUMENT_RESPONSE) {
        return flatbuffers::GetRoot<krypto::serialization::InstrumentResponse>(msg.data());
    }

    return std::nullopt;
}

std::optional<krypto::instruments::ServerParser::receive_variant_t>
krypto::instruments::ServerParser::parse(const zmq::message_t &msg, krypto::utils::MsgType msg_type) {
    if (msg_type == krypto::utils::MsgType::INSTRUMENT_REQUEST) {
        return flatbuffers::GetRoot<krypto::serialization::InstrumentRequest>(msg.data());
    } else if (msg_type == krypto::utils::MsgType::INSTRUMENT_CACHE_REFRESH_REQUEST) {
        return flatbuffers::GetRoot<krypto::serialization::InstrumentCacheRefreshRequest>(msg.data());
    }
    return std::nullopt;
}
