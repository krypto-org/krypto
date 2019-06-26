#pragma once

#include "instrument.h"

namespace krypto::common {
    struct Startup {
        static void init() {
            ExchangeTypeEnum::init();
            InstrumentTypeEnum::init();
            CurrencyEnum::init();
        }
    };
}