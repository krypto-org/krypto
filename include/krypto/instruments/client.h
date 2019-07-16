#pragma once

#include <krypto/network/rpc/client.h>
#include <krypto/common/instrument.h>
#include <krypto/serialization/serialization_generated.h>

namespace krypto::instruments {
    class InstrumentClient final : public krypto::network::rpc::ClientBase<InstrumentClient> {
    public:
        using krypto::network::rpc::ClientBase<InstrumentClient>::ClientBase;
        using krypto::network::rpc::ClientBase<InstrumentClient>::connect;

        std::vector<krypto::common::Instrument> query_all();

        void serialize(krypto::serialization::RequestType requestType);
    };
}
