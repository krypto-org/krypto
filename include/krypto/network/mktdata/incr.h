#pragma once

#include <tuple>
#include <vector>
#include <cmath>

#include <krypto/network/publisher.h>
#include <krypto/types.h>
#include <krypto/serialization/serialization_generated.h>

namespace krypto::network::mktdata {

    class IncrementalPublisher final
            : public krypto::network::PublisherBase<IncrementalPublisher> {

    public:
        using krypto::network::PublisherBase<IncrementalPublisher>::PublisherBase;

        using krypto::network::PublisherBase<IncrementalPublisher>::send;

        void serialize(krypto::SecurityId id, krypto::Timestamp timestamp,
                             const std::vector<std::tuple<Price, krypto::Quantity, krypto::serialization::OrderSide>> &);
    };

}