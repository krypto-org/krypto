#pragma once

#include <future>

#include <krypto/network/rpc/client.h>
#include <krypto/serialization/serialization_generated.h>
#include <krypto/utils/common.h>
#include <krypto/utils/types.h>
#include <krypto/network/helper.h>

namespace {
    const auto convert_inst_type = krypto::utils::convert_compatible_enum<
            krypto::serialization::InstrumentType, krypto::serialization::InstrumentType>;

    const auto convert_currency = krypto::utils::convert_compatible_enum<
            krypto::serialization::Currency, krypto::serialization::Currency>;
}

namespace krypto::instruments {

    class InstrumentClient final {
    private:
        std::unique_ptr<zmq::socket_t> socket_;
        std::string server_endpoint_;
        flatbuffers::FlatBufferBuilder fb_builder_;

        void serialize(krypto::serialization::RequestType request_type);

        static std::vector<krypto::utils::Instrument> process(
                const krypto::serialization::InstrumentResponse *response);

    public:
        InstrumentClient(zmq::context_t &context, const krypto::Config &config);

        std::future<std::vector<krypto::utils::Instrument>> query(
                krypto::serialization::RequestType request_type =
                krypto::serialization::RequestType::RequestType_ALL,
                int timeout = -1);

        void refresh_cache();
    };
}
