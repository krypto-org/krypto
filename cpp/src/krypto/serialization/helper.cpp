#include <krypto/serialization/helper.h>

namespace krypto::serialization {
    void serialize(flatbuffers::FlatBufferBuilder& buffer_builder,
            const krypto::utils::OrderUpdate& order_update) {
        buffer_builder.Clear();
        auto oid_offset = buffer_builder.CreateString(order_update.order_id);
        auto ex_oid_offset = buffer_builder.CreateString(order_update.exchange_order_id);
        krypto::serialization::OrderUpdateBuilder ou_builder{buffer_builder};
        ou_builder.add_timestamp(order_update.timestamp);
        ou_builder.add_filled_quantity(order_update.filled_quantity);
        ou_builder.add_status(order_update.status);
        ou_builder.add_order_id(oid_offset);
        ou_builder.add_exchange_order_id(ex_oid_offset);
        auto ou = ou_builder.Finish();
        buffer_builder.Finish(ou);
    }
}
