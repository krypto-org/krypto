#pragma once

#include <krypto/logger.h>
#include <krypto/utils/common.h>
#include <krypto/instruments/exchanges/instrument.h>
#include <krypto/instruments/exchanges/coinbase.h>
#include <krypto/utils/types.h>

namespace krypto::instruments {

    class ExchangeInstrumentsFactory {
    public:
        template <typename... Args>
        static auto make(const krypto::serialization::Exchange et, Args... args) {
            if (et == exchanges::CoinbaseInstruments::key) {
                return exchanges::CoinbaseInstruments(args...);
            } else {
                throw;
            }
        }
    };

}
