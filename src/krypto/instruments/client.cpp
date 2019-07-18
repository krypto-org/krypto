#include <krypto/instruments/client.h>


namespace {
    const auto convert_inst_type = krypto::utils::convert_compatible_enum<
            krypto::serialization::InstrumentType, krypto::common::InstrumentType>;

    const auto convert_currency = krypto::utils::convert_compatible_enum<
            krypto::serialization::Currency, krypto::common::Currency>;
}


std::vector<krypto::common::Instrument> krypto::instruments::InstrumentClient::query_all()  {
    auto response = send<krypto::serialization::InstrumentResponse, krypto::serialization::RequestType>(
            "instruments", krypto::serialization::RequestType::RequestType_ALL);

    std::vector<krypto::common::Instrument> result;

    if (response) {
        auto insts = response->instruments();
        KRYP_LOG(info, "Received {} Instruments", insts->Length());
        for (size_t i = 0; i < insts->Length(); ++i) {
            auto inst = insts->Get(i);
            krypto::common::Instrument to_insert{
                static_cast<uint64_t>(inst->id()),
                convert_inst_type(inst->type()),
                inst->symbol()->str(),
                krypto::common::ExchangeTypeEnum::names_to_enums.at(inst->exchange()->str()),
                inst->exchange_symbol()->str(),
                inst->tick_size(),
                inst->min_size(),
                inst->max_size(),
                convert_currency(inst->crypto_base()),
                convert_currency(inst->crypto_quote())
                };

            KRYP_LOG(info, "{}", to_insert);
//            result.push_back(inst);
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
