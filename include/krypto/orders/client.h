#pragma once

#include <krypto/network/subscriber.h>
#include <krypto/network/rpc/client.h>
#include "protocol.h"

namespace krypto::orders {
    using received_variant_t = std::variant<OrderRequest, OrderCancelRequest, OrderReplaceRequest>;

    template<typename Derived>
    class OrderClient :
            public krypto::network::rpc::ClientBase<
                    OrderClient<Derived>, received_variant_t,
                    ClientParser, true>,
            public krypto::network::Subscriber<OrderClient<Derived>, ClientParser, true> {
    private:
        // Order Cache
    public:
        explicit OrderClient(const Config &config);

        void process(const krypto::serialization::OrderUpdate *order_update);

        void send_order(const OrderRequest& request);

        void cancel_order(const std::string& order_id);

        void replace_order(const OrderReplaceRequest& request);

        void serialize(const OrderRequest &request);

        void serialize(const OrderCancelRequest &request);

        void serialize(const OrderReplaceRequest &request);
    };

    template<typename Derived>
    OrderClient<Derived>::OrderClient(const Config &config) :
            OrderClient<Derived>::ClientBase{config},
            OrderClient<Derived>::Subscriber(
                    config.at<std::string>(
                            "/services/publisher/orders/proxy/backend/client")) {}

    template<typename Derived>
    void OrderClient<Derived>::process(const krypto::serialization::OrderUpdate *order_update) {
        // derived.orderUpdated();
    }

    template<typename Derived>
    void OrderClient<Derived>::serialize(const OrderRequest &request) {

    }

    template<typename Derived>
    void OrderClient<Derived>::serialize(const OrderCancelRequest &request) {

    }

    template<typename Derived>
    void OrderClient<Derived>::serialize(const OrderReplaceRequest &request) {

    }

}