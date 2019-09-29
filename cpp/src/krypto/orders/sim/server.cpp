#include <krypto/orders/sim/server.h>
#include <krypto/utils/date_time.h>
#include <krypto/utils/types.h>
#include <krypto/utils/common.h>

void krypto::orders::sim::serialize_order_update(
        flatbuffers::FlatBufferBuilder &builder,
        const krypto::utils::OrderUpdate &order_update) {
    builder.Clear();
    auto oid_offset = builder.CreateString(order_update.order_id);
    krypto::serialization::OrderUpdateBuilder ou_builder{builder};
    ou_builder.add_timestamp(order_update.timestamp);
    ou_builder.add_filled_quantity(order_update.filled_quantity);
    ou_builder.add_status(order_update.status);
    ou_builder.add_order_id(oid_offset);
    auto ou = ou_builder.Finish();
    builder.Finish(ou);
}

krypto::orders::sim::OrderServer::OrderServer(zmq::context_t &context, const krypto::Config &config,
                                              std::string &&service) :
        OrderServer::WorkerBase(context, config, std::move(service)),
        publisher_{context, config.at<std::string>(
                "/services/publisher/orders/proxy/frontend/client")} {
    publisher_.connect();
}


krypto::utils::MsgType krypto::orders::sim::OrderServer::process(const krypto::serialization::OrderRequest *request) {
    KRYP_LOG(info, "Processing new order request : {}", request->order_id()->str());
    const krypto::utils::OrderUpdate in_flight{
            krypto::utils::current_time_in_nanoseconds(),
            request->order_id()->str(),
            krypto::serialization::OrderStatus::OrderStatus_IN_FLIGHT,
            0};
    std::string topic = krypto::utils::create_topic(
            krypto::utils::MsgType::ORDER_UPDATE,
            request->security_id());
    publisher_.send(topic, in_flight);

    const krypto::utils::OrderUpdate accept{
            krypto::utils::current_time_in_nanoseconds(),
            request->order_id()->str(),
            krypto::serialization::OrderStatus::OrderStatus_ACCEPTED,
            0
    };
    publisher_.send(topic, accept);

    quotes_accessor_t a;
    if (quotes_.find(a, request->security_id())) {
        auto q = a->second;
        if ((request->side() == krypto::serialization::Side_BUY &&
             request->price() >= q.ask) ||
            (request->side() == krypto::serialization::Side_SELL &&
             request->price() <= q.bid)) {

            KRYP_LOG(info, "=> {} -- filled", request->order_id()->str());
            const krypto::utils::OrderUpdate fill{
                    krypto::utils::current_time_in_nanoseconds(),
                    request->order_id()->str(),
                    krypto::serialization::OrderStatus::OrderStatus_FILLED,
                    request->quantity()
            };
            publisher_.send(topic, fill);
            return krypto::utils::MsgType::NO_PAYLOAD;
        } else {
            if (request->tif() == krypto::serialization::TimeInForce::TimeInForce_IOC) {
                const krypto::utils::OrderUpdate expired{
                        krypto::utils::current_time_in_nanoseconds(),
                        request->order_id()->str(),
                        krypto::serialization::OrderStatus::OrderStatus_EXPIRED,
                        0
                };
                publisher_.send(topic, expired);
                return krypto::utils::MsgType::NO_PAYLOAD;
            } else {
                day_orders_accessor_t acc;
                if (!day_orders_.find(acc, request->security_id())) {
                    day_orders_.insert(acc, request->security_id());
                }

                krypto::utils::OrderRequest r{
                        request->timestamp(),
                        request->security_id(),
                        request->price(),
                        request->quantity(),
                        request->side(),
                        request->order_id()->str(),
                        request->tif()
                };

                acc->second.push_back(r);

                const krypto::utils::OrderUpdate new_order{
                        krypto::utils::current_time_in_nanoseconds(),
                        request->order_id()->str(),
                        krypto::serialization::OrderStatus::OrderStatus_NEW,
                        0
                };
                publisher_.send(topic, new_order);
            }
        }
    }

    const krypto::utils::OrderUpdate rejected{
            krypto::utils::current_time_in_nanoseconds(),
            request->order_id()->str(),
            krypto::serialization::OrderStatus::OrderStatus_REJECTED,
            request->quantity()
    };
    publisher_.send(topic, rejected);

    return krypto::utils::MsgType::NO_PAYLOAD;
}

krypto::utils::MsgType
krypto::orders::sim::OrderServer::process(const krypto::serialization::OrderCancelRequest *request) {
    KRYP_LOG(info, "Processing cancel order request : {}", request->order_id()->str());
    const krypto::utils::OrderUpdate in_flight{
            krypto::utils::current_time_in_nanoseconds(),
            request->order_id()->str(),
            krypto::serialization::OrderStatus::OrderStatus_CANCEL_IN_FLIGHT,
            0};
    std::string topic = krypto::utils::create_topic(
            krypto::utils::MsgType::ORDER_UPDATE,
            0);
    publisher_.send(topic, in_flight);
    return krypto::utils::MsgType::NO_PAYLOAD;
}

krypto::utils::MsgType
krypto::orders::sim::OrderServer::process(const krypto::serialization::OrderReplaceRequest *request) {
    KRYP_LOG(info, "Processing replace order request : {}", request->order_id()->str());
    const krypto::utils::OrderUpdate in_flight{
            krypto::utils::current_time_in_nanoseconds(),
            request->order_id()->str(),
            krypto::serialization::OrderStatus::OrderStatus_REPLACE_IN_FLIGHT,
            0};
    std::string topic = krypto::utils::create_topic(
            krypto::utils::MsgType::ORDER_UPDATE,
            0);
    publisher_.send(topic, in_flight);
    return krypto::utils::MsgType::NO_PAYLOAD;
}

void krypto::orders::sim::OrderServer::fill_price(
        const int64_t security_id,
        const krypto::serialization::Side &side,
        const int64_t price) {
}


void krypto::orders::sim::OrderUpdatePublisher::serialize(
        const krypto::utils::OrderUpdate &order_update) {
    serialize_order_update(fb_builder_, order_update);
}
