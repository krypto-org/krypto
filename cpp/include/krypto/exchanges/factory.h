#pragma once

#include <krypto/logger.h>
#include <krypto/utils/common.h>
#include <krypto/exchanges/base.h>
#include <krypto/exchanges/coinbase.h>

namespace krypto::exchanges {

    class ExchangeFactory {
    public:
        template <typename... Args>
        static auto make(const krypto::utils::ExchangeType et, Args... args) {
            if (et == Coinbase::key) {
                return Coinbase(args...);
            } else {
                throw;
            }
        }
    };

}
