#pragma once

#include <krypto/network/subscriber.h>
#include <krypto/network/rpc/client.h>
#include <krypto/utils/id_gen.h>
#include <krypto/utils/date_time.h>
#include <krypto/utils/message_types.h>
#include <krypto/orders/protocol.h>
#include <bits/unordered_set.h>

namespace krypto::orders {
    using received_variant_t = std::variant<krypto::utils::OrderRequest, krypto::utils::OrderCancelRequest, krypto::utils::OrderReplaceRequest>;

    class OrderClient final :
            public krypto::network::rpc::ClientBase<OrderClient, received_variant_t, ClientParser, true>,
            public krypto::network::Subscriber<OrderClient, ClientParser, true> {

    public:
        using listener_func_t = std::function<void(const krypto::serialization::OrderUpdate *)>;
        using krypto::network::rpc::ClientBase<OrderClient, received_variant_t, ClientParser, true>::send;
        using krypto::network::rpc::ClientBase<OrderClient, received_variant_t, ClientParser, true>::receive;
        using krypto::network::Subscriber<OrderClient, ClientParser, true>::subscribe;

    private:
        static constexpr const size_t ORDER_ID_LENGTH = 12;
        krypto::utils::OrderIdGenerator<ORDER_ID_LENGTH> order_id_generator_;
        std::vector<listener_func_t> callbacks_;
        // Order State
    public:
        OrderClient(zmq::context_t& context, const Config &config);

        void process(const krypto::serialization::OrderUpdate *order_update);

        std::string new_order(const std::string& service,
                int64_t security_id, int64_t price, int64_t size,
                       krypto::serialization::Side,
                       krypto::serialization::TimeInForce = krypto::serialization::TimeInForce::TimeInForce_DAY);

        void cancel_order(const std::string& service, const std::string& order_id);

        void replace_order(const std::string& service,
                const std::string& order_id,
                int64_t price, int64_t size,
                krypto::serialization::Side);

        void serialize(const krypto::utils::OrderRequest &request);

        void serialize(const krypto::utils::OrderCancelRequest &request);

        void serialize(const krypto::utils::OrderReplaceRequest &request);

        void register_listener(listener_func_t&& listener);

        void register_listener(const listener_func_t& listener);
    };
}