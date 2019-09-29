#include <krypto/orders/protocol.h>
#include <krypto/utils/types.h>

std::optional<krypto::orders::ServerParser::receive_variant_t>
krypto::orders::ServerParser::parse(const zmq::message_t &msg, krypto::utils::MsgType msg_type) {
    if (msg_type == krypto::utils::MsgType::ORDER_REQUEST) {
        return flatbuffers::GetRoot<krypto::serialization::OrderRequest>(msg.data());
    } else if (msg_type == krypto::utils::MsgType::ORDER_CANCEL_REQUEST) {
        return flatbuffers::GetRoot<krypto::serialization::OrderCancelRequest>(msg.data());
    } else if (msg_type == krypto::utils::MsgType::ORDER_REPLACE_REQUEST) {
        return flatbuffers::GetRoot<krypto::serialization::OrderReplaceRequest>(msg.data());
    }
    return std::nullopt;
}

std::optional<krypto::orders::ClientParser::receive_variant_t>
krypto::orders::ClientParser::parse(const zmq::message_t &msg, krypto::utils::MsgType msg_type) {
    if (msg_type == krypto::utils::MsgType::ORDER_UPDATE) {
        return flatbuffers::GetRoot<krypto::serialization::OrderUpdate>(msg.data());
    }
    return std::nullopt;
}
