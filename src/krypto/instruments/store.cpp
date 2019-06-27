#include <krypto/instruments/store.h>
#include <krypto/common/startup.h>
#include <krypto/config.h>
#include <krypto/exchanges/factory.h>

krypto::instruments::InstrumentStore::InstrumentStore(const krypto::Config &config) :
        config_{config} {
}

std::vector<krypto::common::Instrument> krypto::instruments::InstrumentStore::load() {
    std::vector<krypto::common::Instrument> result;
    for (auto && et : krypto::common::ExchangeTypeEnum::values) {
        auto exchange = krypto::exchanges::ExchangeFactory::make(et, config_);
        auto instruments = exchange.instruments();

        result.insert(result.end(), std::begin(instruments), std::end(instruments));

        KRYP_LOG(info, "Loaded {} Instruments from {}", instruments.size(),
                 krypto::common::ExchangeTypeEnum::enum_to_names.at(et));
    }
    return result;
}

