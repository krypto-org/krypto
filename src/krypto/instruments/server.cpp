#include <utility>

#include <utility>
#include <krypto/instruments/server.h>
#include <krypto/utils/enum.h>

namespace {
    const auto convert_inst_type = krypto::utils::convert_compatible_enum<
            krypto::utils::InstrumentType, krypto::serialization::InstrumentType>;

    const auto convert_currency = krypto::utils::convert_compatible_enum<
            krypto::utils::Currency, krypto::serialization::Currency>;
}

krypto::utils::MsgType krypto::instruments::Server::process(const krypto::serialization::InstrumentRequest *) {
    fb_builder_.Clear();
    std::vector<flatbuffers::Offset<krypto::serialization::Instrument>> instruments;
    for (auto &&inst : cache_) {
        auto inst_offset = krypto::serialization::CreateInstrument(
                fb_builder_, inst.id, ::convert_inst_type(inst.inst_type),
                fb_builder_.CreateString(inst.symbol),
                fb_builder_.CreateString(
                        krypto::utils::ExchangeTypeEnum::enum_to_names.at(inst.exchange)),
                fb_builder_.CreateString(inst.exchange_symbol),
                inst.tick_size,
                inst.min_size,
                inst.max_size,
                ::convert_currency(inst.crypto_base),
                ::convert_currency(inst.crypto_quote));
        instruments.emplace_back(inst_offset);
    }
    auto instruments_offset = fb_builder_.CreateVector(instruments);
    krypto::serialization::InstrumentResponseBuilder builder{fb_builder_};
    builder.add_instruments(instruments_offset);
    auto response = builder.Finish();
    fb_builder_.Finish(response);

    return krypto::utils::MsgType::INSTRUMENT_RESPONSE;
}

krypto::instruments::Server::Server(const krypto::Config &config, std::string service) :
        WorkerBase(config, std::move(service)), store_{config} {
    cache_ = store_.load();
}
