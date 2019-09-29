#pragma once

#include <cstdint>
#include <krypto/utils/common.h>
#include <krypto/utils/types.h>

namespace krypto::exchanges {

    template<typename ExchangeImpl>
    class Exchange {
    protected:
        std::unordered_map<std::string, krypto::serialization::Currency> names_to_currency_reference_;
    public:
        Exchange();

        std::vector<krypto::utils::Instrument> instruments();
    };

    template<typename ExchangeImpl>
    std::vector<krypto::utils::Instrument> Exchange<
            ExchangeImpl>::instruments() {
        auto &derived = static_cast<ExchangeImpl &>(*this);
        return derived.read_instruments();
    }

    template<typename ExchangeImpl>
    Exchange<ExchangeImpl>::Exchange() :
            names_to_currency_reference_{krypto::utils::name_to_currency()} {}
}
