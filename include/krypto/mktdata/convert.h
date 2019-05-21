#pragma once

#include <cmath>

namespace krypto::mktdata {
    constexpr int64_t PRICE_CONVERSION = 10000000000;

    constexpr int64_t QUANTITY_CONVERSION = 10000000000;

    constexpr int64_t convert_price(const double_t price) {
        return static_cast<int64_t>(PRICE_CONVERSION * price);
    }

    constexpr int64_t convert_quantity(const double_t quantity) {
        return static_cast<int64_t>(QUANTITY_CONVERSION * quantity);
    }
}




