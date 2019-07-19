#pragma once

#include <cstdint>
#include <unordered_map>
#include <krypto/utils/enum.h>
#include <string>
#include <cmath>
#include <spdlog/fmt/fmt.h>
#include <ostream>
#include <krypto/serialization/serialization_generated.h>

namespace krypto::utils {
    constexpr const uint64_t exchange_base = 100;

    enum struct ExchangeType : uint8_t {
        COINBASE,
        SIZE
    };

    using ExchangeTypeEnum = krypto::utils::Enum<ExchangeType>;

    constexpr auto exchange_id(const ExchangeType et) {
        return static_cast<uint64_t>(et) + exchange_base + 1;
    }

    enum class InstrumentType : uint8_t {
        UNKNOWN,
        STOCK,
        FUTURE,
        OPTION,
        FOREX,
        CRYPTO,
        SIZE
    };

    using InstrumentTypeEnum = Enum<InstrumentType>;

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
        LINK,
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

    using CurrencyEnum = Enum<Currency>;

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

    const std::string instrument_symbol(const std::string& base, const std::string& quote);

    struct Startup {
        static void init();
    };
}
