#include <krypto/utils/common.h>

namespace krypto::utils {
    std::string instrument_symbol(const std::string &base, const std::string &quote) {
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
                        "ZRX",
                        "XTZ",
                        "ALGO",
                        "DASH"
                };
        return names;
    }

    template<>
    std::array<std::string, enum_size<MsgType>()> enum_names<MsgType>() {
        std::array<std::string, enum_size<MsgType>()> names =
                {
                        "",
                        "A",
                        "Q",
                        "T",
                        "H",
                        "I_REQ",
                        "I_RES",
                        "I_CACHE_REQ"
                };
        return names;
    }

    void Startup::init() {
        ExchangeTypeEnum::init();
        InstrumentTypeEnum::init();
        CurrencyEnum::init();
        MsgTypeEnum::init();
    }

    const std::string create_topic(const MsgType msg_type, const uint64_t id) {
        auto prefix = enum_names<MsgType>()[static_cast<uint8_t>(msg_type)];
        return prefix + std::to_string(id);
    }
}