#include <krypto/utils/common.h>

namespace krypto::utils {

    std::string instrument_symbol(const std::string &base, const std::string &quote) {
        return fmt::format("{0}_{1}", base, quote);
    }


    std::string create_topic(const MsgType msg_type, const int64_t id) {
        auto prefix = MsgTypeNames[static_cast<uint8_t>(msg_type)];
        return prefix + std::to_string(id);
    }

    std::string create_topic(const MsgType msg_type, const krypto::serialization::Exchange exchange) {
        auto prefix = std::string{MsgTypeNames[static_cast<uint8_t>(msg_type)]};
        return prefix + "_" + std::string{krypto::serialization::EnumNameExchange(exchange)};
    }


    std::unordered_map<std::string, krypto::serialization::Currency> name_to_currency() noexcept {
        auto values = krypto::serialization::EnumValuesCurrency();
        std::unordered_map<std::string, krypto::serialization::Currency> result;
        for (int i = 0; i <= static_cast<int>(krypto::serialization::Currency::Currency_MAX); ++i) {
            result[krypto::serialization::EnumNameCurrency(values[i])] = values[i];
        }
        return result;
    }

    std::unordered_map<std::string, krypto::utils::MsgType > name_to_msg_type() {
        auto values = krypto::utils::MsgTypes;
        std::unordered_map<std::string, krypto::utils::MsgType > result;
        for (int i = 0; i < enum_size<MsgType>(); ++i) {
            result[krypto::utils::MsgTypeNames[i]] = values[i];
        }
        return result;
    }
}