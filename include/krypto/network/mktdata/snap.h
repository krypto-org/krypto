#pragma once

#include <krypto/network/publisher.h>
#include <krypto/types.h>


namespace krypto::network::mktdata {

    class SnapshotPublisher : public krypto::network::PublisherBase<SnapshotPublisher> {
    public:
        using krypto::network::PublisherBase<SnapshotPublisher>::PublisherBase;

        void serialize(krypto::secid_t id, krypto::timestamp_t timestamp,
                             const std::vector<std::tuple<krypto::price_t, krypto::quantity_t >> & bids,
                             const std::vector<std::tuple<krypto::price_t, krypto::quantity_t >> & asks);
    };
}

