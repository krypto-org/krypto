#pragma once

#include <string>
#include <cstdint>
#include <cmath>
#include <krypto/common/exchange.h>
#include <spdlog/fmt/fmt.h>
#include <ostream>

namespace krypto::common {
    enum class InstrumentType : uint8_t {
        UNKNOWN,
        STOCK,
        FUTURE,
        OPTION,
        FOREX,
        CRYPTO,
        SIZE
    };

    using InstrumentTypeEnum = krypto::utils::Enum<InstrumentType>;

    enum class Currency : uint8_t {
        UNKNOWN,
        BAT,
        BCH,
        BTC,
        CVC,
        DAI,
        DNT,
        EOS,
        ETC,
        ETH,
        EUR,
        GBP,
        GNT,
        LOOM,
        LTC,
        MANA,
        REP,
        USD,
        USDC,
        XLM,
        XRP,
        ZEC,
        ZRX,
        SIZE
    };

    using CurrencyEnum = krypto::utils::Enum<Currency>;

    struct Instrument {
        uint64_t id;
        InstrumentType inst_type;
        std::string symbol;
        ExchangeType exchange;
        std::string exchange_symbol;
        double_t tick_size;
        double_t min_size;
        double_t max_size;
        Currency crypto_base;
        Currency crypto_quote;

        friend std::ostream &operator<<(std::ostream &os, const Instrument &instrument) {
            os << "id: " << instrument.id  << " symbol: " << instrument.symbol
            << " exchange_symbol: " << instrument.exchange_symbol
               << " tick_size: " << instrument.tick_size << " min_size: " << instrument.min_size << " max_size: "
               << instrument.max_size;
            return os;
        }
    };

    constexpr uint64_t instrument_id(const ExchangeType et, const Currency base, const Currency quote) {
        return exchange_id(et) * 100000000 + static_cast<uint64_t>(base) * 10000 +
               static_cast<uint64_t>(quote);
    }

    const auto instrument_symbol(const std::string& base, const std::string& quote) {
        return fmt::format("{0}_{1}", base, quote);
    }
}


namespace krypto::utils {
    template<>
    std::array<std::string, enum_size<krypto::common::InstrumentType>()> enum_names<krypto::common::InstrumentType>() {
        std::array<std::string, enum_size<krypto::common::InstrumentType>()> names =
                {
                        "UNKNOWN",
                        "STOCK",
                        "FUTURE",
                        "OPTION",
                        "FOREX",
                        "CRYPTO"
                };
        return names;
    }

    template<>
    std::array<std::string, enum_size<krypto::common::Currency>()> enum_names<krypto::common::Currency>() {
        std::array<std::string, enum_size<krypto::common::Currency>()> names =
                {
                        "UNKNOWN",
                        "BAT",
                        "BCH",
                        "BTC",
                        "CVC",
                        "DAI",
                        "DNT",
                        "EOS",
                        "ETC",
                        "ETH",
                        "EUR",
                        "GBP",
                        "GNT",
                        "LOOM",
                        "LTC",
                        "MANA",
                        "REP",
                        "USD",
                        "USDC",
                        "XLM",
                        "XRP",
                        "ZEC",
                        "ZRX"
                };
        return names;
    }
}