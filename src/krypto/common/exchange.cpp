#include <array>
#include <krypto/common/exchange.h>

namespace krypto::utils {
    template <>
    std::array<std::string, enum_size<krypto::common::ExchangeType>()> enum_names<krypto::common::ExchangeType>() {
        std::array<std::string, enum_size<krypto::common::ExchangeType>()> names =
                                        {"COINBASE"};
        return names;
    }
}
