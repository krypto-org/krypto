#pragma once

#include <krypto/utils/types.h>

namespace krypto::instruments::exchanges {
    std::vector<krypto::utils::Instrument> parse(nlohmann::json insts_json,
                                                 const std::unordered_map<std::string, krypto::serialization::Currency> &currency_reference);

    std::vector<krypto::utils::Instrument> parse(nlohmann::json insts_json,
                                                 const std::unordered_map<std::string, krypto::serialization::Currency> &currency_reference) {
        std::vector<krypto::utils::Instrument> result;

        for (auto &&product: insts_json) {
            krypto::utils::Instrument inst{};

            auto base_currency = product.at("base_currency").get<std::string>();
            auto quote_currency = product.at("quote_currency").get<std::string>();

            if (currency_reference.find(base_currency) == currency_reference.end()) {
                KRYP_LOG(warn, "{} not found in currency reference", base_currency);
                continue;
            }

            if (currency_reference.find(quote_currency) == currency_reference.end()) {
                KRYP_LOG(warn, "{} not found in currency reference", quote_currency);
                continue;
            }

            auto symbol = krypto::utils::instrument_symbol(base_currency, quote_currency);
            auto exch_symbol = product.at("id").get<std::string>();
            auto tick_size = std::stod(product.at("base_increment").get<std::string>());
            auto min_size = std::stod(product.at("base_min_size").get<std::string>());
            auto max_size = std::stod(product.at("base_max_size").get<std::string>());
            auto base = currency_reference.at(base_currency);
            auto quote = currency_reference.at(quote_currency);

            auto inst_type = krypto::serialization::InstrumentType::InstrumentType_CRYPTO;
            const auto key = krypto::serialization::Exchange_COINBASE;

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
                              quote,
                              true,
                              true}
            );
        }

        return result;
    }
}