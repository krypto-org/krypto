#include <krypto/risk/position_manager.h>
#include <krypto/utils/date_time.h>
#include <krypto/mktdata/convert.h>

namespace krypto::risk {

    PositionManager::PositionManager(krypto::instruments::InstrumentClient instrument_client,
                                     const std::string &redis_endpoint) :
            redis_{redis_endpoint} {
        auto received = instrument_client.query(
                krypto::serialization::RequestType_ALL, 1000).get();
        if (received.empty()) {
            KRYP_LOG(info, "No instruments received");
        }
        std::for_each(received.cbegin(), received.cend(), [=](auto &&instr) {
            if (instr.active) {
                instruments_[instr.id] = instr;
                mark_price_[instr.id] = std::numeric_limits<double_t>::quiet_NaN();
            }
        });
    }

    const std::vector<krypto::utils::Fill> &PositionManager::fills() const {
        return fills_;
    }


    void PositionManager::add_fill(const utils::Fill &fill) {
        fills_.push_back(fill);
        const int32_t multiplier = fill.side == krypto::serialization::Side::Side_BUY ? 1 : -1;
        const int64_t security_id = fill.security_id;
        if (risk_summary_.position_by_security_id.find(security_id) ==
            risk_summary_.position_by_security_id.end()) {
            risk_summary_.position_by_security_id.insert({security_id,
                                                          {krypto::utils::current_time_in_nanoseconds(),
                                                                security_id, 0, 0,
                                                                std::numeric_limits<double_t>::quiet_NaN()}});
        }

        // UPDATE Overall positions
        const auto position_change = (multiplier * fill.quantity);
        risk_summary_.position_by_security_id[security_id].position += position_change;

        // UPDATE Trader positions
        if (risk_summary_.trader_positions.find(fill.trader) == risk_summary_.trader_positions.end()) {
            risk_summary_.trader_positions.insert({fill.trader, {}});
        }
        position_map *trader_positions = &risk_summary_.trader_positions.at(fill.trader);
        if (trader_positions->find(security_id) == trader_positions->end()) {
            trader_positions->insert({security_id,
                                      {krypto::utils::current_time_in_nanoseconds(), security_id, 0, 0,
                                       std::numeric_limits<double_t>::quiet_NaN()}});
        }
        trader_positions->at(security_id).position += position_change;

        // UPDATE Book positions
        if (risk_summary_.book_positions.find(fill.book) == risk_summary_.book_positions.end()) {
            risk_summary_.book_positions.insert({fill.book, {}});
        }
        position_map *book_positions = &risk_summary_.book_positions.at(fill.book);
        if (book_positions->find(security_id) == book_positions->end()) {
            book_positions->insert({security_id,
                                    {krypto::utils::current_time_in_nanoseconds(), security_id, 0, 0,
                                     std::numeric_limits<double_t>::quiet_NaN()}});
        }
        book_positions->at(security_id).position += position_change;
    }

    void PositionManager::update_pnl() {
        for (auto&&[security_id, instr]: instruments_) {
            std::unordered_map<std::string, double_t> values;
            redis_.hgetall(std::to_string(security_id), std::inserter(values, values.begin()));
            const double_t bid = values["bid"];
            const double_t offer = values["offer"];
            mark_price_[security_id] = (bid + offer) / 2.0;
        }

        for (auto &&fill: fills_) {
            const auto security_id = fill.security_id;
            const auto quantity = fill.quantity;
            const auto previous_mark_price = std::isnan(
                    risk_summary_.position_by_security_id[security_id].mark_price) ?
                                             fill.price
                                                                                   : risk_summary_.position_by_security_id[security_id].mark_price;
            const double_t pnl_change = quantity * (mark_price_[security_id] - previous_mark_price);

            risk_summary_.position_by_security_id[security_id].pnl += pnl_change;
            risk_summary_.trader_positions[fill.trader][security_id].pnl += pnl_change;
            risk_summary_.book_positions[fill.book][security_id].pnl += pnl_change;
            risk_summary_.pnl += pnl_change;
            risk_summary_.timestamp = krypto::utils::current_time_in_nanoseconds();
        }
    }

    const utils::RiskSummary &PositionManager::risk_summary() const {
        return risk_summary_;
    }
}
