#include <krypto/utils/common.h>

namespace krypto::utils {
    const std::string instrument_symbol(const std::string &base, const std::string &quote) {
        return fmt::format("{0}_{1}", base, quote);
    }

    template<>
    std::array<std::string, enum_size<ExchangeType>()> enum_names<ExchangeType>() {
        std::array<std::string, enum_size<ExchangeType>()> names =
                {"COINBASE"};
        return names;
    }

    template<>
    std::array<std::string, enum_size<InstrumentType>()> enum_names<InstrumentType>() {
        std::array<std::string, enum_size<InstrumentType>()> names =
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
    std::array<std::string, enum_size<Currency>()> enum_names<Currency>() {
        std::array<std::string, enum_size<Currency>()> names =
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
                        "LINK",
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

    void Startup::init() {
        ExchangeTypeEnum::init();
        InstrumentTypeEnum::init();
        CurrencyEnum::init();
    }
}