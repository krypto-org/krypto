#pragma once

#include <krypto/common/instrument.h>
#include <krypto/network/rpc/worker.h>
#include <krypto/config.h>
#include <krypto/instruments/store.h>
#include <krypto/serialization/serialization_generated.h>

namespace krypto::instruments {
    class Server final : public krypto::network::rpc::WorkerBase<Server, krypto::serialization::InstrumentRequest,
            krypto::serialization::InstrumentResponse> {
    private:
        InstrumentStore store_;
        std::vector<krypto::common::Instrument> cache_;
    public:
        Server(zmq::context_t &context,
                               std::string broker,
                               std::string service,
                               bool verbose, const krypto::Config& config);

        using krypto::network::rpc::WorkerBase<Server, krypto::serialization::InstrumentRequest,
                krypto::serialization::InstrumentResponse>::start;

        void process(const krypto::serialization::InstrumentRequest *);
    };
}


