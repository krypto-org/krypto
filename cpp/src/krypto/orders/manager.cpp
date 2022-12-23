
#include <krypto/mktdata/convert.h>
#include <krypto/orders/manager.h>
#include <krypto/logger.h>

std::optional<krypto::utils::Fill>
krypto::orders::OrderManager::process_update(const krypto::serialization::OrderUpdate *order_update) {
    const auto status = order_update->status();

    switch (status) {
        case krypto::serialization::OrderStatus::OrderStatus_ACCEPTED:
            process_accept(order_update);
            return std::nullopt;
        case krypto::serialization::OrderStatus::OrderStatus_CANCELLED:
            process_cancel(order_update);
            return std::nullopt;
        case krypto::serialization::OrderStatus::OrderStatus_REPLACED:
            process_replace(order_update);
            return std::nullopt;
        case krypto::serialization::OrderStatus::OrderStatus_PARTIALLY_FILLED:
        case krypto::serialization::OrderStatus::OrderStatus_FILLED:
            return process_filled(order_update);
        default:
            KRYP_LOG(debug, "Ignoring order update");
            break;
    }
    return std::nullopt;
}

void krypto::orders::OrderManager::process_accept(const krypto::serialization::OrderUpdate *order_update) {
    if (!order_exists(order_update)) {
        return;
    }
    krypto::utils::Order *order = &orders_.at(order_update->order_id()->str());
    order->order_id = order_update->exchange_order_id()->str();
    order->status = order_update->status();
    order->last_update_timestamp = order_update->timestamp();
}

void krypto::orders::OrderManager::process_cancel(const krypto::serialization::OrderUpdate *order_update) {
    if (!order_exists(order_update)) {
        return;
    }
    krypto::utils::Order *order = &orders_.at(order_update->order_id()->str());
    if (order->filled_quantity == 0) {
        orders_.erase(order_update->order_id()->str());
        return;
    }
    order->status = order_update->status();
    order->last_update_timestamp = order_update->timestamp();
}

void krypto::orders::OrderManager::process_replace(const krypto::serialization::OrderUpdate *order_update) {
    // TODO: Handle Replace Order
}

std::optional<krypto::utils::Fill>
krypto::orders::OrderManager::process_filled(const krypto::serialization::OrderUpdate *order_update) {
    if (!order_exists(order_update)) {
        return std::nullopt;
    }
    krypto::utils::Order *order = &orders_.at(order_update->order_id()->str());
    const double_t filled_price = krypto::mktdata::extract_price(order_update->filled_price());
    const double_t filled_qty = krypto::mktdata::extract_quantity(order_update->filled_quantity());
    const krypto::utils::Fill fill{order->security_id, order->cl_order_id, order->side, filled_price, filled_qty,
                                   order->size - filled_qty, order->trader, order->book};
    order->fills.push_back(fill);
    return fill;
}

void krypto::orders::OrderManager::register_order(const krypto::serialization::OrderRequest *order_request) {
    const std::string order_id = order_request->order_id()->str();
    const krypto::utils::Order order{order_id, order_request->security_id(), "",
                                     krypto::mktdata::extract_price(order_request->price()),
                                     krypto::mktdata::extract_quantity(order_request->quantity()),
                                     order_request->side(), order_request->tif(), order_request->trader()->str(),
                                     order_request->book()->str()};
    orders_.insert({order_id, order});
}

bool krypto::orders::OrderManager::order_exists(const krypto::serialization::OrderUpdate *order_update) {
    const std::string order_id = order_update->order_id()->str();
    if (orders_.find(order_id) == orders_.end()) {
        KRYP_LOG(warn, "Order not found for client order id - {}", order_id);
        return false;
    }
    return true;
}

const std::unordered_map<std::string, krypto::utils::Order> &krypto::orders::OrderManager::orders() const {
    return orders_;
};


