#include <krypto/config.h>
#include <krypto/exchanges/factory.h>
#include <krypto/common/startup.h>



int main(int argc, char **argv) {

    krypto::common::Startup::init();

    const krypto::Config config(argv[1]);

    for (auto && et : krypto::common::ExchangeTypeEnum::values) {
        auto exchange = krypto::exchanges::ExchangeFactory::make(et, config);
        auto instruments = exchange.instruments();

        KRYP_LOG(info, "Loaded {} Instruments from {}", instruments.size(),
                krypto::common::ExchangeTypeEnum::enum_to_names.at(et));
    }
}