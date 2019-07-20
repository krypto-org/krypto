#include <krypto/instruments/client.h>


namespace {
    const auto convert_inst_type = krypto::utils::convert_compatible_enum<
            krypto::serialization::InstrumentType, krypto::utils::InstrumentType>;

    const auto convert_currency = krypto::utils::convert_compatible_enum<
            krypto::serialization::Currency, krypto::utils::Currency>;
}


std::vector<krypto::utils::Instrument> krypto::instruments::InstrumentClient::query_all()  {
    auto response = send<krypto::serialization::InstrumentResponse, krypto::serialization::RequestType>(
            "instruments", krypto::serialization::RequestType::RequestType_ALL);

    std::vector<krypto::utils::Instrument> result;

    if (response) {
        auto insts = response->instruments();
        KRYP_LOG(info, "Received {} instruments", insts->Length());
        for (size_t i = 0; i < insts->Length(); ++i) {
            auto inst = insts->Get(i);
            krypto::utils::Instrument to_insert{
                static_cast<uint64_t>(inst->id()),
                convert_inst_type(inst->type()),
                inst->symbol()->str(),
                krypto::utils::ExchangeTypeEnum::names_to_enums.at(inst->exchange()->str()),
                inst->exchange_symbol()->str(),
                inst->tick_size(),
                inst->min_size(),
                inst->max_size(),
                convert_currency(inst->crypto_base()),
                convert_currency(inst->crypto_quote())
                };
            result.push_back(to_insert);
        }

    }
    return result;
}

void krypto::instruments::InstrumentClient::serialize(
        const krypto::serialization::RequestType requestType) {
    krypto::serialization::InstrumentRequestBuilder builder{fb_builder_};
    builder.add_type(requestType);
    auto req = builder.Finish();
    fb_builder_.Finish(req);
}
