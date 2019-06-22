#pragma once

#include <cstdint>

#include <krypto/serialization/serialization_generated.h>

namespace krypto {
    enum struct ExchangeType : uint8_t {
        COINBASE,
        BINANCE,
        GEMINI
    };

    template <typename ExchangeImpl>
    class Exchange {
    public:
        const std::vector<krypto::serialization::Instrument> instruments();
    };

    template<typename ExchangeImpl>
    const std::vector<krypto::serialization::Instrument> Exchange<ExchangeImpl>::instruments() {
        auto &derived = static_cast<ExchangeImpl &>(*this);
        return derived.read_instruments();
    }
}
