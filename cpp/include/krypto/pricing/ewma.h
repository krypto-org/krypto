#pragma once

#include <cmath>

namespace krypto::pricing {
    class EWMACalculator final {
    private:
        double_t half_life_;
        double_t decay_coefficient_;
        double_t calculate_multiplier(double_t time_delta);
    public:
        explicit EWMACalculator(double_t half_life);

        static double_t calculate_wma(
                double_t bid,
                double_t bid_qty,
                double_t ask,
                double_t ask_qty);

        double_t calculate_mean(
                double_t previous_ewma,
                double_t current_price,
                double_t time_delta);

        double_t calculate_std(double_t previous_std,
                               double_t current_price,
                               double_t time_delta,
                               double_t current_ewma);

    };
}