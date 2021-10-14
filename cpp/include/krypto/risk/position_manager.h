#pragma once

#include <krypto/instruments/client.h>
#include <sw/redis++/redis++.h>

namespace krypto::risk {
    class PositionManager final {
    private:
        using position_map = std::unordered_map<int64_t, krypto::utils::Position>;
        std::unordered_map<int64_t, krypto::utils::Instrument> instruments_;
        std::unordered_map<int64_t, double_t> mark_price_{};
        std::vector<krypto::utils::Fill> fills_;
        krypto::utils::RiskSummary risk_summary_;
        sw::redis::Redis redis_;
    public:
        PositionManager(krypto::instruments::InstrumentClient instrument_client, const std::string& redis_endpoint);

        void add_fill(const krypto::utils::Fill& fill);

        void update_pnl();

        const std::vector<krypto::utils::Fill> &fills() const;

        const utils::RiskSummary &risk_summary() const;

    };
}