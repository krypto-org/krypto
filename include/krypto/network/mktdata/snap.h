#pragma once

#include <krypto/network/publisher.h>
#include <krypto/types.h>


namespace krypto::network::mktdata {

    class SnapshotPublisher : public krypto::network::PublisherBase<SnapshotPublisher,
            krypto::SecurityId , krypto::Timestamp,
            std::vector<std::tuple<krypto::Price, krypto::Quantity >>,
            std::vector<std::tuple<krypto::Price, krypto::Quantity >>> {
    public:
        using krypto::network::PublisherBase<SnapshotPublisher,
                krypto::SecurityId , krypto::Timestamp,
                std::vector<std::tuple<krypto::Price, krypto::Quantity >>,
                std::vector<std::tuple<krypto::Price , krypto::Quantity >>>::PublisherBase;

        void generate_buffer(krypto::SecurityId id, krypto::Timestamp timestamp,
                             const std::vector<std::tuple<krypto::Price, krypto::Quantity >> & bids,
                             const std::vector<std::tuple<krypto::Price, krypto::Quantity >> & asks);
    };
}

