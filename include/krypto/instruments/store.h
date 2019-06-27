#pragma once

#include <vector>
#include <krypto/config.h>
#include <krypto/common/instrument.h>

namespace krypto::instruments {
    class InstrumentStore final {
        const krypto::Config& config_;
    public:
        explicit InstrumentStore(const krypto::Config& config);

        std::vector<krypto::common::Instrument> load();
    };
}