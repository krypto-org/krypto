#pragma once

#include <vector>
#include <krypto/config.h>
#include <krypto/utils/common.h>

namespace krypto::instruments {
    class InstrumentLoader final {
        const krypto::Config& config_;
    public:
        explicit InstrumentLoader(const krypto::Config& config);

        std::vector<krypto::utils::Instrument> load();
    };
}