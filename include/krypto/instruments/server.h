#pragma once

#include <krypto/network/rpc/worker.h>
#include <krypto/serialization/serialization_generated.h>

namespace krypto::instruments {
    class Server final : public krypto::network::rpc::WorkerBase<Server, krypto::serialization::InstrumentRequest,
            krypto::serialization::InstrumentResponse> {
    public:
        using krypto::network::rpc::WorkerBase<Server, krypto::serialization::InstrumentRequest,
                krypto::serialization::InstrumentResponse>::WorkerBase;
        using krypto::network::rpc::WorkerBase<Server, krypto::serialization::InstrumentRequest,
                krypto::serialization::InstrumentResponse>::start;

        void process(const krypto::serialization::InstrumentRequest *);
    };
}


