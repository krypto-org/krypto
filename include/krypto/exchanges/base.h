#pragma once

#include <cstdint>
#include <krypto/common/instrument.h>

namespace krypto::exchanges {

    template <typename ExchangeImpl>
    class Exchange {
    public:
        const std::vector<krypto::common::Instrument> instruments();
    };

    template<typename ExchangeImpl>
    const std::vector<krypto::common::Instrument> Exchange<
            ExchangeImpl>::instruments() {
        auto &derived = static_cast<ExchangeImpl &>(*this);
        return derived.read_instruments();
    }
}
