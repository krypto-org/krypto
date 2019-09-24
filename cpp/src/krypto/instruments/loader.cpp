#include <krypto/instruments/loader.h>
#include <krypto/utils/common.h>
#include <krypto/config.h>
#include <krypto/exchanges/factory.h>

krypto::instruments::InstrumentLoader::InstrumentLoader(const krypto::Config &config) :
        config_{config} {
}

std::vector<krypto::utils::Instrument> krypto::instruments::InstrumentLoader::load() {
    std::vector<krypto::utils::Instrument> result;
    for (auto && et : krypto::utils::ExchangeTypeEnum::values) {
        auto exchange = krypto::exchanges::ExchangeFactory::make(et, config_);
        auto instruments = exchange.instruments();

        result.insert(result.end(), std::begin(instruments), std::end(instruments));

        KRYP_LOG(info, "Loaded {} Instruments from {}", instruments.size(),
                 krypto::utils::ExchangeTypeEnum::enum_to_names.at(et));
    }
    return result;
}

