#pragma once

#include <cmath>
#include <string>

namespace krypto::exchanges::coinbase {
    struct Order final {
        std::string product_id;
        std::string side;
        double_t price;
        double_t size;
        std::string order_type;
    };
}
