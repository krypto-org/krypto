#pragma once

#include <krypto/network/rpc/client.h>
#include <krypto/utils/common.h>
#include <krypto/serialization/serialization_generated.h>

namespace krypto::instruments {
    class InstrumentClient final : public krypto::network::rpc::ClientBase<InstrumentClient, true> {
    private:
        std::vector<krypto::utils::Instrument> instruments_;
    public:
        using krypto::network::rpc::ClientBase<InstrumentClient, true>::ClientBase;
        using krypto::network::rpc::ClientBase<InstrumentClient, true>::connect;

        std::vector<krypto::utils::Instrument> query_all();

        void process_response(const krypto::serialization::InstrumentResponse *response);

        void serialize(krypto::serialization::RequestType requestType);
    };
}
