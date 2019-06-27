#pragma once

#include <krypto/common/instrument.h>

namespace krypto::common {
    struct Startup {
        static void init() {
            ExchangeTypeEnum::init();
            InstrumentTypeEnum::init();
            CurrencyEnum::init();
        }
    };
}