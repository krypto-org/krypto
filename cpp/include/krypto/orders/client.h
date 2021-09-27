#pragma once

#include <unordered_set>

#include <krypto/network/subscriber.h>
#include <krypto/network/rpc/client.h>
#include <krypto/utils/order_id.h>
#include <krypto/utils/date_time.h>
#include <krypto/orders/protocol.h>

namespace krypto::orders {
    class OrderClient final {
    public:
        using order_update_func_t = std::function<void(const krypto::serialization::OrderUpdate *)>;
    private:
        static constexpr const size_t ORDER_ID_LENGTH = 12;
        static constexpr const char *queue_endpoint = "inproc://orders_queue";
        std::unique_ptr<zmq::socket_t> sender_;
        std::unique_ptr<zmq::socket_t> queue_push_;
        std::unique_ptr<zmq::socket_t> queue_pull_;
        std::string gateway_endpoint_;
        std::atomic_bool running_;
        std::vector<order_update_func_t> callbacks_;
        krypto::utils::OrderIdGenerator<ORDER_ID_LENGTH> order_id_generator_;
        flatbuffers::FlatBufferBuilder fb_builder_;

        void serialize(const krypto::utils::OrderRequest &request);

        void serialize(const krypto::utils::OrderCancelRequest &request);

        void serialize(const krypto::utils::OrderReplaceRequest &request);

        void process(const krypto::serialization::OrderUpdate *order_update);

        void poll_sockets();

    public:
        OrderClient(zmq::context_t &context, const krypto::Config &config);

        std::string new_order(const std::string &,
                              int64_t, int64_t, int64_t,
                              krypto::serialization::Side,
                              krypto::serialization::TimeInForce =
                              krypto::serialization::TimeInForce::TimeInForce_DAY);

        void cancel_order(const std::string &, const std::string &);

        void replace_order(const std::string &,
                           const std::string &,
                           int64_t,
                           int64_t,
                           krypto::serialization::Side);

        void register_listener(const order_update_func_t &listener);

        void unregister_listener(const order_update_func_t &listener);

        std::thread start();

        void stop();
    };
}