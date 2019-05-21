#pragma once

#include <tuple>
#include <vector>
#include <cmath>

#include <krypto/network/publisher.h>
#include <krypto/types.h>
#include <krypto/serialization/serialization_generated.h>

namespace krypto::network::mktdata {

    class IncrementalPublisher final
            : public krypto::network::PublisherBase<
                    IncrementalPublisher,
                    krypto::SecurityId,
                    krypto::Timestamp,
                    std::vector<std::tuple<krypto::Price, krypto::Quantity, krypto::serialization::OrderSide>>> {

    public:
        using krypto::network::PublisherBase<IncrementalPublisher,
                krypto::SecurityId,
                krypto::Timestamp,
                std::vector<std::tuple<krypto::Price, krypto::Quantity, krypto::serialization::OrderSide>>>::PublisherBase;

        using krypto::network::PublisherBase<IncrementalPublisher,
                krypto::SecurityId,
                krypto::Timestamp,
                std::vector<std::tuple<krypto::Price, krypto::Quantity, krypto::serialization::OrderSide>>>::send;

        void generate_buffer(krypto::SecurityId id, krypto::Timestamp timestamp,
                             const std::vector<std::tuple<Price, krypto::Quantity, krypto::serialization::OrderSide>> &);
    };

}