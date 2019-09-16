#pragma once

#include <string>

#include <krypto/utils/common.h>
#include <krypto/serialization/serialization_generated.h>

namespace krypto::instruments {
    struct InstrumentRequest {
        krypto::serialization::RequestType request_type;
        static const constexpr krypto::utils::MsgType message_type =
                krypto::utils::MsgType::INSTRUMENT_REQUEST;
    };
}
