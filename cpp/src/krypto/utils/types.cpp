#include <krypto/utils/types.h>

namespace krypto::utils {
    const std::unordered_map<std::string, krypto::utils::MsgType> MessageTypeData::msg_type_reference{
            name_to_msg_type()};
}
