#pragma once

#include <cstdint>
#include <krypto/utils/common.h>
#include <krypto/utils/types.h>

namespace krypto::instruments::exchanges {

    template<typename Impl>
    class ExchangeInstruments {
    protected:
        static std::unordered_map<std::string, krypto::serialization::Currency> currency_reference;
    public:
        ExchangeInstruments() = default;

        std::vector<krypto::utils::Instrument> instruments();
    };

    template<typename Impl>
    std::vector<krypto::utils::Instrument> ExchangeInstruments<
            Impl>::instruments() {
        auto &derived = static_cast<Impl &>(*this);
        return derived.read_instruments();
    }

    template<typename Impl>
    std::unordered_map<std::string, krypto::serialization::Currency> ExchangeInstruments<Impl>::currency_reference = krypto::utils::name_to_currency();
}
