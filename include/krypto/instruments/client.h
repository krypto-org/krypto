#pragma once

#include <krypto/network/rpc/client.h>
#include <krypto/utils/common.h>
#include <krypto/instruments/message_types.h>
#include <krypto/serialization/serialization_generated.h>

namespace krypto::instruments {
    using received_variant_t = std::variant<InstrumentRequest>;

    class InstrumentClient final : public krypto::network::rpc::ClientBase<InstrumentClient,received_variant_t, true> {
    private:
        std::vector<krypto::utils::Instrument> instruments_;
    public:
        using krypto::network::rpc::ClientBase<InstrumentClient, received_variant_t, true>::ClientBase;
        using krypto::network::rpc::ClientBase<InstrumentClient, received_variant_t, true>::connect;

        std::vector<krypto::utils::Instrument> query_all();

        void process_response(const krypto::serialization::InstrumentResponse *response);

        void serialize(const InstrumentRequest& request);
    };
}
