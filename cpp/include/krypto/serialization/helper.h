#pragma once

#include <krypto/utils/types.h>

namespace krypto::serialization {
    void serialize(flatbuffers::FlatBufferBuilder &buffer_builder,
                   const krypto::utils::OrderUpdate &order_update);

    void
    serialize(flatbuffers::FlatBufferBuilder &buffer_builder, const krypto::utils::RiskSummary &risk_summary);


}
