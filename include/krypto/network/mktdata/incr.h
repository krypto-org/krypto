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

        void serialize(krypto::secid_t id, krypto::timestamp_t timestamp,
                             const std::vector<std::tuple<price_t, krypto::quantity_t, krypto::serialization::OrderSide>> &);
    };

}