#pragma once

#include <krypto/network/publisher.h>
#include <krypto/types.h>


namespace krypto::network::mktdata {

    class SnapshotPublisher : public krypto::network::PublisherBase<SnapshotPublisher> {
    public:
        using krypto::network::PublisherBase<SnapshotPublisher>::PublisherBase;

        void serialize(krypto::SecurityId id, krypto::Timestamp timestamp,
                             const std::vector<std::tuple<krypto::Price, krypto::Quantity >> & bids,
                             const std::vector<std::tuple<krypto::Price, krypto::Quantity >> & asks);
    };
}

