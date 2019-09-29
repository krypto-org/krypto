#include <krypto/instruments/loader.h>
#include <krypto/config.h>
#include <krypto/exchanges/factory.h>
#include <krypto/utils/types.h>

krypto::instruments::InstrumentLoader::InstrumentLoader(const krypto::Config &config) :
        config_{config} {
}

std::vector<krypto::utils::Instrument> krypto::instruments::InstrumentLoader::load() {
    std::vector<krypto::utils::Instrument> result;
    for (auto &&et : krypto::serialization::EnumValuesExchange()) {
        auto exchange = krypto::exchanges::ExchangeFactory::make(et, config_);
        auto instruments = exchange.instruments();

        result.insert(result.end(), std::begin(instruments), std::end(instruments));

        KRYP_LOG(info, "Loaded {} Instruments from {}", instruments.size(),
                 krypto::serialization::EnumNamesExchange()[static_cast<uint8_t>(et)]);
    }
    return result;
}

