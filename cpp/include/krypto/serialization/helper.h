#pragma once

#include <krypto/utils/message_types.h>

namespace krypto::serialization {
    void serialize(flatbuffers::FlatBufferBuilder& buffer_builder,
            const krypto::utils::OrderUpdate& order_update);
}
