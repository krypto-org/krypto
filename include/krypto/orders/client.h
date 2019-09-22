#pragma once

#include <krypto/network/subscriber.h>
#include <krypto/network/rpc/client.h>
#include <krypto/utils/id_gen.h>
#include <krypto/utils/date_time.h>
#include <krypto/orders/protocol.h>

namespace krypto::orders {
    using received_variant_t = std::variant<OrderRequest, OrderCancelRequest, OrderReplaceRequest>;

    template<typename Derived>
    class OrderClient :
            public krypto::network::rpc::ClientBase<
                    OrderClient<Derived>, received_variant_t,
                    ClientParser, true>,
            public krypto::network::Subscriber<OrderClient<Derived>, ClientParser, true> {
    private:
        static constexpr const size_t ORDER_ID_LENGTH = 12;
        krypto::utils::OrderIdGenerator<ORDER_ID_LENGTH> order_id_generator_;
        // Order State
    public:
        OrderClient(zmq::context_t& context, const Config &config);

        void process(const krypto::serialization::OrderUpdate *order_update);

        void new_order(const std::string& service,
                uint64_t security_id, int64_t price, int64_t size,
                       krypto::serialization::Side,
                       krypto::serialization::TimeInForce = krypto::serialization::TimeInForce::TimeInForce_DAY);

        void cancel_order(const std::string& service, const std::string& order_id);

        void replace_order(const std::string& service,
                const std::string& order_id,
                int64_t price, int64_t size,
                krypto::serialization::Side);

        void serialize(const OrderRequest &request);

        void serialize(const OrderCancelRequest &request);

        void serialize(const OrderReplaceRequest &request);
    };

    template<typename Derived>
    OrderClient<Derived>::OrderClient(zmq::context_t& context, const Config &config) :
            OrderClient<Derived>::ClientBase{context, config},
            OrderClient<Derived>::Subscriber(context,
                    config.at<std::string>(
                            "/services/publisher/orders/proxy/backend/client")) {}

    template<typename Derived>
    void OrderClient<Derived>::process(const krypto::serialization::OrderUpdate *order_update) {
        KRYP_LOG(info, "Received Order Update");
        // derived.orderUpdated();
    }

    template<typename Derived>
    void OrderClient<Derived>::serialize(const OrderRequest &request) {
        auto oid_offset = OrderClient<Derived>::ClientBase::fb_builder_.CreateString(
                request.order_id);

        krypto::serialization::OrderRequestBuilder builder{
                OrderClient<Derived>::ClientBase::fb_builder_};
        builder.add_order_id(oid_offset);
        builder.add_timestamp(request.timestamp);
        builder.add_security_id(request.security_id);
        builder.add_price(request.price);
        builder.add_quantity(request.quantity);
        builder.add_side(request.side);
        builder.add_tif(request.tif);

        auto req = builder.Finish();
        OrderClient<Derived>::ClientBase::fb_builder_.Finish(req);
    }

    template<typename Derived>
    void OrderClient<Derived>::serialize(const OrderCancelRequest &request) {
        auto oid_offset = OrderClient<Derived>::ClientBase::fb_builder_.CreateString(
                request.order_id);
        krypto::serialization::OrderCancelRequestBuilder builder{
                OrderClient<Derived>::ClientBase::fb_builder_};
        builder.add_order_id(oid_offset);
        builder.add_timestamp(request.timestamp);
        auto req = builder.Finish();
        OrderClient<Derived>::ClientBase::fb_builder_.Finish(req);
    }

    template<typename Derived>
    void OrderClient<Derived>::serialize(const OrderReplaceRequest &request) {
        auto oid_offset = OrderClient<Derived>::ClientBase::fb_builder_.CreateString(
                request.order_id);

        krypto::serialization::OrderReplaceRequestBuilder builder{
                OrderClient<Derived>::ClientBase::fb_builder_};
        builder.add_order_id(oid_offset);
        builder.add_timestamp(request.timestamp);
        builder.add_price(request.price);
        builder.add_quantity(request.quantity);
        builder.add_side(request.side);

        auto req = builder.Finish();
        OrderClient<Derived>::ClientBase::fb_builder_.Finish(req);
    }

    template<typename Derived>
    void OrderClient<Derived>::cancel_order(
            const std::string& service, const std::string &order_id) {
        OrderCancelRequest cancel{
                krypto::utils::current_time_in_nanoseconds(),
                order_id};
        OrderClient<Derived>::ClientBase::send(service, cancel);

        OrderClient<Derived>::ClientBase::receive(-1);
    }

    template<typename Derived>
    void OrderClient<Derived>::replace_order(
            const std::string& service,
            const std::string &order_id, int64_t price, int64_t size,
            krypto::serialization::Side side) {
        OrderReplaceRequest replace_request{
                krypto::utils::current_time_in_nanoseconds(),
                order_id,
                price,
                size,
                side};
        OrderClient<Derived>::ClientBase::send(service, replace_request);

        OrderClient<Derived>::ClientBase::receive(-1);
    }

    template<typename Derived>
    void OrderClient<Derived>::new_order(
            const std::string &service,
            uint64_t security_id,
            int64_t price,
            int64_t size,
            krypto::serialization::Side side,
            krypto::serialization::TimeInForce tif) {
        auto oid = order_id_generator_.new_id();
        OrderRequest order_request{
                krypto::utils::current_time_in_nanoseconds(),
                security_id,
                price,
                size,
                side,
                oid,
                tif};
        OrderClient<Derived>::ClientBase::send(service, order_request);
        OrderClient<Derived>::ClientBase::receive(-1);
    }

}