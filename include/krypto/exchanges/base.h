#pragma once

#include <cstdint>
#include <krypto/utils/common.h>

namespace krypto::exchanges {

    template <typename ExchangeImpl>
    class Exchange {
    public:
        const std::vector<krypto::utils::Instrument> instruments();
    };

    template<typename ExchangeImpl>
    const std::vector<krypto::utils::Instrument> Exchange<
            ExchangeImpl>::instruments() {
        auto &derived = static_cast<ExchangeImpl &>(*this);
        return derived.read_instruments();
    }
}
