#include <krypto/pricing/ewma.h>
#include <krypto/logger.h>

double_t
krypto::pricing::EWMACalculator::calculate_wma(
        double_t bid, double_t bid_qty,
        double_t ask, double_t ask_qty) {
    return ((bid * ask_qty) + (ask * bid_qty)) / (bid_qty + ask_qty);
}

krypto::pricing::EWMACalculator::EWMACalculator(const double_t half_life) :
        half_life_{half_life}, decay_coefficient_{std::log(2.0) / half_life} {
}

double_t krypto::pricing::EWMACalculator::calculate_multiplier(double_t time_delta) {

    return std::exp(-decay_coefficient_ * time_delta);;
}

double_t krypto::pricing::EWMACalculator::calculate_mean(
        double_t previous_ewma, double_t current_price, double_t time_delta) {
    auto curr_mult = calculate_multiplier(time_delta);
    return curr_mult * previous_ewma + (1 - curr_mult) * current_price;
}

double_t
krypto::pricing::EWMACalculator::calculate_std(
        double_t previous_std, double_t current_price,
        double_t time_delta, double_t current_ewma) {
    auto curr_mult = calculate_multiplier(time_delta);
    auto prev_var = std::pow(previous_std, 2);
    auto curr_var = curr_mult * std::pow(current_price - current_ewma, 2) +
                    (1 - curr_mult) * prev_var;
    return std::sqrt(curr_var);
}


