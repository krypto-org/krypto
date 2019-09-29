#pragma once

#include <vector>

#include <krypto/utils/common.h>
#include <krypto/exchanges/base.h>
#include <krypto/http.h>
#include <nlohmann/json.hpp>
#include <krypto/utils/types.h>

namespace krypto::exchanges {
    class Coinbase : public Exchange<Coinbase> {
    private:
        krypto::HttpClient http_client_;
        std::string endpoint_;
    public:
        explicit Coinbase(const Config &config);

        std::vector<krypto::utils::Instrument> read_instruments();

        static const krypto::serialization::Exchange key = krypto::serialization::Exchange::Exchange_COINBASE;
    };

    Coinbase::Coinbase(const Config &config) : Coinbase::Exchange(),
                                               http_client_{
                                                       config.at<std::string>("/exchanges/coinbase/rest/base_url")},
                                               endpoint_{config.at<std::string>(
                                                       "/exchanges/coinbase/rest/channels/instruments")} {}

    std::vector<krypto::utils::Instrument> Coinbase::read_instruments() {
        std::vector<krypto::utils::Instrument> result;

        auto insts = http_client_.get("/" + endpoint_);

        if (insts.has_value()) {
            nlohmann::json insts_json = nlohmann::json::parse(insts.value());

            for (auto &&product : insts_json) {
                krypto::utils::Instrument inst{};

                KRYP_LOG(info, product.dump());

                auto base_currency = product.at("base_currency").get<std::string>();
                auto quote_currency = product.at("quote_currency").get<std::string>();
                auto symbol = krypto::utils::instrument_symbol(base_currency, quote_currency);
                auto exch_symbol = product.at("id").get<std::string>();
                auto tick_size = std::stod(product.at("base_increment").get<std::string>());
                auto min_size = std::stod(product.at("base_min_size").get<std::string>());
                auto max_size = std::stod(product.at("base_max_size").get<std::string>());
                auto base = names_to_currency_reference_.at(base_currency);
                auto quote = names_to_currency_reference_.at(quote_currency);

                auto inst_type = krypto::serialization::InstrumentType::InstrumentType_CRYPTO;

                auto id = krypto::utils::instrument_id(key, base, quote);


                result.push_back({id,
                                  inst_type,
                                  symbol,
                                  key,
                                  exch_symbol,
                                  tick_size,
                                  min_size,
                                  max_size,
                                  base,
                                  quote}
                );
            }
        }

        return result;
    }

}