#pragma once

#include <krypto/utils/common.h>
#include <krypto/network/rpc/worker.h>
#include <krypto/config.h>
#include <krypto/instruments/loader.h>
#include <krypto/serialization/serialization_generated.h>

namespace krypto::instruments {
    class Server final : public krypto::network::rpc::WorkerBase<Server, krypto::serialization::InstrumentRequest,
            krypto::serialization::InstrumentResponse> {
    private:
        InstrumentLoader store_;
        std::vector<krypto::utils::Instrument> cache_;
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


