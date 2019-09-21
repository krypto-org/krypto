#include <krypto/orders/sim/server.h>

krypto::orders::sim::OrderServer::OrderServer(const krypto::Config &config, std::string&& service) :
        OrderServer::WorkerBase(config, std::move(service)),
        publisher_{config.at<std::string>(
                "/services/publisher/orders/proxy/frontend/client")} {}


krypto::utils::MsgType krypto::orders::sim::OrderServer::process(const krypto::serialization::OrderRequest *) {
    return krypto::utils::MsgType::ORDER_UPDATE;
}

krypto::utils::MsgType krypto::orders::sim::OrderServer::process(const krypto::serialization::OrderCancelRequest *) {
    return krypto::utils::MsgType::ORDER_UPDATE;
}

krypto::utils::MsgType krypto::orders::sim::OrderServer::process(const krypto::serialization::OrderReplaceRequest *) {
    return krypto::utils::MsgType::ORDER_UPDATE;
}

void krypto::orders::sim::OrderServer::fill_price(
        const int64_t security_id, const krypto::serialization::Side &side, const int64_t price) {
}


void krypto::orders::sim::OrderUpdatePublisher::serialize(
        const krypto::orders::OrderUpdate &order_update) {
    fb_builder_.Clear();
    auto oid_offset = fb_builder_.CreateString(order_update.order_id);
    krypto::serialization::OrderUpdateBuilder ou_builder{fb_builder_};
    ou_builder.add_timestamp(order_update.timestamp);
    ou_builder.add_filled_quantity(order_update.filled_quantity);
    ou_builder.add_status(krypto::orders::serialize(order_update.status));
    ou_builder.add_order_id(oid_offset);
    auto ou = ou_builder.Finish();
    fb_builder_.Finish(ou);
}
