#include <krypto/orders/client.h>


krypto::orders::OrderClient::OrderClient(zmq::context_t& context, const Config &config) :
        krypto::orders::OrderClient::ClientBase{context, config},
        krypto::orders::OrderClient::Subscriber(context,
                                         config.at<std::string>(
                                                 "/services/publisher/orders/proxy/backend/client")) {}

void krypto::orders::OrderClient::process(const krypto::serialization::OrderUpdate *order_update) {
    std::for_each(std::begin(callbacks_), std::end(callbacks_), [&] (auto&& c) {
        c(order_update);
    });
}

void krypto::orders::OrderClient::serialize(const krypto::utils::OrderRequest &request) {
    auto oid_offset = krypto::orders::OrderClient::ClientBase::fb_builder_.CreateString(
            request.order_id);

    krypto::serialization::OrderRequestBuilder builder{
            krypto::orders::OrderClient::ClientBase::fb_builder_};
    builder.add_order_id(oid_offset);
    builder.add_timestamp(request.timestamp);
    builder.add_security_id(request.security_id);
    builder.add_price(request.price);
    builder.add_quantity(request.quantity);
    builder.add_side(request.side);
    builder.add_tif(request.tif);

    auto req = builder.Finish();
    krypto::orders::OrderClient::ClientBase::fb_builder_.Finish(req);
}

void krypto::orders::OrderClient::serialize(const krypto::utils::OrderCancelRequest &request) {
    auto oid_offset = krypto::orders::OrderClient::ClientBase::fb_builder_.CreateString(
            request.order_id);
    krypto::serialization::OrderCancelRequestBuilder builder{
            krypto::orders::OrderClient::ClientBase::fb_builder_};
    builder.add_order_id(oid_offset);
    builder.add_timestamp(request.timestamp);
    auto req = builder.Finish();
    krypto::orders::OrderClient::ClientBase::fb_builder_.Finish(req);
}

void krypto::orders::OrderClient::serialize(const krypto::utils::OrderReplaceRequest &request) {
    auto oid_offset = krypto::orders::OrderClient::ClientBase::fb_builder_.CreateString(
            request.order_id);

    krypto::serialization::OrderReplaceRequestBuilder builder{
            krypto::orders::OrderClient::ClientBase::fb_builder_};
    builder.add_order_id(oid_offset);
    builder.add_timestamp(request.timestamp);
    builder.add_price(request.price);
    builder.add_quantity(request.quantity);
    builder.add_side(request.side);

    auto req = builder.Finish();
    krypto::orders::OrderClient::ClientBase::fb_builder_.Finish(req);
}

void krypto::orders::OrderClient::cancel_order(
        const std::string& service, const std::string &order_id) {
    krypto::utils::OrderCancelRequest cancel{
            krypto::utils::current_time_in_nanoseconds(),
            order_id};
    send(service, cancel);
    receive(-1);
}

void krypto::orders::OrderClient::replace_order(
        const std::string& service,
        const std::string &order_id, int64_t price, int64_t size,
        krypto::serialization::Side side) {
    krypto::utils::OrderReplaceRequest replace_request{
            krypto::utils::current_time_in_nanoseconds(),
            order_id,
            price,
            size,
            side};
    send(service, replace_request);
    receive(-1);
}

std::string krypto::orders::OrderClient::new_order(
        const std::string &service,
        int64_t security_id,
        int64_t price,
        int64_t size,
        krypto::serialization::Side side,
        krypto::serialization::TimeInForce tif) {
    auto oid = order_id_generator_.new_id();
    krypto::utils::OrderRequest order_request{
            krypto::utils::current_time_in_nanoseconds(),
            security_id,
            price,
            size,
            side,
            oid,
            tif};
    send(service, order_request);
    receive(-1);
    return oid;
}

void krypto::orders::OrderClient::register_listener(krypto::orders::OrderClient::listener_func_t &&listener) {
    callbacks_.push_back(std::move(listener));
}

void krypto::orders::OrderClient::register_listener(const krypto::orders::OrderClient::listener_func_t &listener) {
    callbacks_.push_back(listener);
}
