#include <krypto/instruments/client.h>

std::vector<krypto::common::Instrument> krypto::instruments::InstrumentClient::query_all() {
    auto response = send<const krypto::serialization::InstrumentResponse*, krypto::serialization::RequestType>(
            "instruments", krypto::serialization::RequestType::RequestType_ALL);
    KRYP_LOG(info, "{}", response == nullptr);
    return std::vector<krypto::common::Instrument>{};
}

void krypto::instruments::InstrumentClient::serialize(
        const krypto::serialization::RequestType requestType) {
    krypto::serialization::InstrumentRequestBuilder builder{fb_builder_};
    builder.add_type(requestType);
    auto req = builder.Finish();
    fb_builder_.Finish(req);
}
