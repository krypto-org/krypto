#pragma once

#include <krypto/network/publisher.h>
#include <krypto/utils/types.h>


namespace krypto::network::mktdata {

    class SnapshotPublisher : public krypto::network::PublisherBase<SnapshotPublisher> {
    public:
        using krypto::network::PublisherBase<SnapshotPublisher>::PublisherBase;

        void serialize(krypto::utils::secid_t id, krypto::utils::timestamp_t timestamp,
                       const std::vector<std::tuple<krypto::utils::price_t,
                               krypto::utils::quantity_t >> &bids,
                       const std::vector<std::tuple<krypto::utils::price_t,
                               krypto::utils::quantity_t >> &asks);
    };
}

