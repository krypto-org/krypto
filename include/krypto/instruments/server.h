#pragma once

#include <krypto/utils/common.h>
#include <krypto/network/rpc/worker.h>
#include <krypto/config.h>
#include <krypto/instruments/loader.h>
#include <krypto/serialization/serialization_generated.h>

namespace krypto::instruments {
    class Server final : public krypto::network::rpc::WorkerBase<Server, krypto::serialization::InstrumentRequest,
            krypto::serialization::InstrumentResponse, true> {
    private:
        InstrumentLoader store_;
        std::vector<krypto::utils::Instrument> cache_;
    public:
        Server(const krypto::Config& config, std::string service);

        using krypto::network::rpc::WorkerBase<Server, krypto::serialization::InstrumentRequest,
                krypto::serialization::InstrumentResponse, true>::start;

        krypto::utils::MsgType process(const krypto::serialization::InstrumentRequest *);
    };
}


