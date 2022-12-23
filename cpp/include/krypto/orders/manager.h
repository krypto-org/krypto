#pragma once

#include <krypto/utils/types.h>
#include <optional>


namespace krypto::orders {
    class OrderManager final {
    private:
        std::unordered_map<std::string, krypto::utils::Order> orders_;

        void process_accept(const krypto::serialization::OrderUpdate *order_update);

        void process_cancel(const krypto::serialization::OrderUpdate *order_update);

        void process_replace(const krypto::serialization::OrderUpdate *order_update);

        std::optional<krypto::utils::Fill> process_filled(const krypto::serialization::OrderUpdate *order_update);

        bool order_exists(const krypto::serialization::OrderUpdate *order_update);

    public:
        void register_order(const krypto::serialization::OrderRequest *order_request);

        std::optional<krypto::utils::Fill> process_update(const krypto::serialization::OrderUpdate *order_update);

        const std::unordered_map<std::string, krypto::utils::Order> &orders() const;
    };
}