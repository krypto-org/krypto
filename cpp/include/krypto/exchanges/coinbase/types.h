#pragma once

#include <cmath>
#include <string>
#include <optional>
#include <ostream>

namespace krypto::exchanges::coinbase {
    struct Order final {
        std::string product_id;
        std::string side;
        double_t price;
        double_t size;
        std::string order_type = "limit";
        std::optional<std::string> client_oid = std::nullopt;
        std::optional<std::string> self_trade_prevention_flag = std::nullopt;
        std::optional<std::string> stop = std::nullopt;
        std::optional<double_t> stop_price = std::nullopt;
        std::optional<std::string> tif = std::nullopt;
        std::optional<std::string> cancel_after = std::nullopt;
        bool post_only = false;

        friend std::ostream &operator<<(std::ostream &os, const Order &order) {
            os << "product_id: " << order.product_id << " side: " << order.side << " price: " << order.price
               << " size: " << order.size << " order_type: " << order.order_type << " post_only: " << order.post_only;
            return os;
        }
    };
}
