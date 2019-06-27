#include <krypto/common/instrument.h>

const std::string krypto::common::instrument_symbol(const std::string& base, const std::string& quote) {
    return fmt::format("{0}_{1}", base, quote);
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
}