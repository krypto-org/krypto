#include <krypto/mktdata/parser.h>


std::optional<krypto::mktdata::Parser::receive_variant_t>
krypto::mktdata::Parser::parse(const zmq::message_t &msg, krypto::utils::MsgType msg_type) {
            switch (msg_type) {
            case krypto::utils::MsgType::QUOTE: {
                return flatbuffers::GetRoot<krypto::serialization::Quote>(msg.data());
            }
            case krypto::utils::MsgType::TRADE: {
                return flatbuffers::GetRoot<krypto::serialization::Trade>(msg.data());
            }
            case krypto::utils::MsgType::HEARTBEAT: {
                return flatbuffers::GetRoot<krypto::serialization::Heartbeat>(msg.data());
            }
            default: {
                break;
            }
        }
        return std::nullopt;
}
