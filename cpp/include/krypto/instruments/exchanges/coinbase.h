#pragma once

#include <vector>

#include <krypto/utils/common.h>
#include <krypto/instruments/exchanges/instrument.h>
#include <krypto/utils/http.h>
#include <nlohmann/json.hpp>
#include <krypto/utils/types.h>
#include <krypto/exchanges/coinbase/api.h>

namespace krypto::instruments::exchanges {
    std::vector<krypto::utils::Instrument> parse(
            nlohmann::json insts_json,
            const std::unordered_map<std::string, krypto::serialization::Currency> &currency_reference);

    class CoinbaseInstruments : public ExchangeInstruments<CoinbaseInstruments> {
    private:
        krypto::exchanges::coinbase::PublicApi api_;
    public:
        explicit CoinbaseInstruments(const krypto::Config &config, const std::string &environment);

        std::vector<krypto::utils::Instrument> read_instruments();

        static const krypto::serialization::Exchange key = krypto::serialization::Exchange::Exchange_COINBASE;
    };
}