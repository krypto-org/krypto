#pragma once

#include <vector>

#include <krypto/utils/common.h>
#include <krypto/instruments/exchanges/instrument.h>
#include <krypto/utils/http.h>
#include <nlohmann/json.hpp>
#include <krypto/utils/types.h>
#include <krypto/exchanges/coinbase/api.h>
#include <krypto/instruments/exchanges/parser.h>

namespace krypto::instruments::exchanges {
    class CoinbaseInstruments : public ExchangeInstruments<CoinbaseInstruments> {
    private:
        krypto::exchanges::coinbase::PublicApi api_;
    public:
        explicit CoinbaseInstruments(const Config &config, const std::string &environment);

        std::vector<krypto::utils::Instrument> read_instruments();

        static const krypto::serialization::Exchange key = krypto::serialization::Exchange::Exchange_COINBASE;
    };

    CoinbaseInstruments::CoinbaseInstruments(const Config &config, const std::string &environment) :
            CoinbaseInstruments::ExchangeInstruments(),
            api_{config, environment} {}

    std::vector<krypto::utils::Instrument> CoinbaseInstruments::read_instruments() {
        auto insts = api_.get_products();
        if (!insts.has_value()) {
            return {};
        }

        return krypto::instruments::exchanges::parse(insts.value(), CoinbaseInstruments::currency_reference);
    }

}