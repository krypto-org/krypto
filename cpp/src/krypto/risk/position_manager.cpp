#include <krypto/risk/position_manager.h>
#include <krypto/utils/date_time.h>

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
            instruments_[instr.id] = instr;
            mark_price_[instr.id] = std::numeric_limits<double_t>::quiet_NaN();
        });
    }

    const std::vector<krypto::utils::Fill> &PositionManager::fills() const {
        return fills_;
    }

    const PositionManager::position_map &PositionManager::position_by_security_id() const {
        return position_by_security_id_;
    }

    const std::unordered_map<std::string, PositionManager::position_map> &PositionManager::position_by_trader() const {
        return position_by_trader_;
    }

    const std::unordered_map<std::string, PositionManager::position_map> &PositionManager::position_by_book() const {
        return position_by_book_;
    }

    void PositionManager::add_fill(const utils::Fill &fill) {
        fills_.push_back(fill);
        const int32_t multiplier = fill.side == krypto::serialization::Side::Side_BUY ? 1 : -1;
        const int64_t security_id = fill.security_id;
        if (position_by_security_id_.find(security_id) == position_by_security_id_.end()) {
            position_by_security_id_.insert({security_id,
                                             {krypto::utils::current_time_in_nanoseconds(), security_id, 0, 0,
                                              std::numeric_limits<double_t>::quiet_NaN()}});
        }

        // UPDATE Overall positions
        const auto position_change = (multiplier * fill.quantity);
        position_by_security_id_[security_id].position += position_change;

        // UPDATE Trader positions
        if (position_by_trader_.find(fill.trader) == position_by_trader_.end()) {
            position_by_trader_.insert({fill.trader, {}});
        }
        position_map *trader_positions = &position_by_trader_.at(fill.trader);
        if (trader_positions->find(security_id) == trader_positions->end()) {
            trader_positions->insert({security_id,
                                      {krypto::utils::current_time_in_nanoseconds(), security_id, 0, 0,
                                       std::numeric_limits<double_t>::quiet_NaN()}});
        }
        trader_positions->at(security_id).position += position_change;

        // UPDATE Book positions
        if (position_by_book_.find(fill.book) == position_by_book_.end()) {
            position_by_book_.insert({fill.book, {}});
        }
        position_map *book_positions = &position_by_book_.at(fill.book);
        if (book_positions->find(security_id) == book_positions->end()) {
            book_positions->insert({security_id,
                                    {krypto::utils::current_time_in_nanoseconds(), security_id, 0, 0,
                                     std::numeric_limits<double_t>::quiet_NaN()}});
        }
        book_positions->at(security_id).position += position_change;
    }

    void PositionManager::update_pnl() {
        // TODO: Query Redis for latest price
        // TODO: Calculate pnl for each fill and update by key
    }
}
