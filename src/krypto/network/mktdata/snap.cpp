#include <krypto/network/mktdata/snap.h>

namespace krypto::network::mktdata {
    void SnapshotPublisher::serialize(
            krypto::secid_t id, krypto::timestamp_t timestamp,
            const std::vector<std::tuple<krypto::price_t, krypto::quantity_t >> &bids,
            const std::vector<std::tuple<krypto::price_t, krypto::quantity_t >> &asks) {

        std::vector<krypto::serialization::SnapshotPriceLevel> bid_levels;
        bid_levels.reserve(bids.size());
        std::vector<krypto::serialization::SnapshotPriceLevel> ask_levels;
        ask_levels.reserve(asks.size());

        for (auto&[price, qty] : bids) {
            bid_levels.emplace_back(krypto::serialization::SnapshotPriceLevel{price, qty});
        }

        for (auto&[price, qty] : asks) {
            ask_levels.emplace_back(krypto::serialization::SnapshotPriceLevel{price, qty});
        }

        auto bids_offset = fb_builder_.CreateVectorOfStructs<krypto::serialization::SnapshotPriceLevel>(bid_levels);
        auto asks_offset = fb_builder_.CreateVectorOfStructs<krypto::serialization::SnapshotPriceLevel>(ask_levels);

        krypto::serialization::SnapshotBuilder snapshot_builder(fb_builder_);
        snapshot_builder.add_security_id(id);
        snapshot_builder.add_timestamp(timestamp);
        snapshot_builder.add_bids(bids_offset);
        snapshot_builder.add_asks(asks_offset);

        auto snapshot = snapshot_builder.Finish();

        fb_builder_.Finish(snapshot);
    }
}



