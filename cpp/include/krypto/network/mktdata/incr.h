#pragma once

#include <tuple>
#include <vector>
#include <cmath>

#include <krypto/network/publisher.h>
#include <krypto/utils/types.h>
#include <krypto/serialization/serialization_generated.h>

namespace krypto::network::mktdata {

    class IncrementalPublisher final
            : public krypto::network::PublisherBase<IncrementalPublisher> {

    public:
        using krypto::network::PublisherBase<IncrementalPublisher>::PublisherBase;

        using krypto::network::PublisherBase<IncrementalPublisher>::send;

        void serialize(krypto::utils::secid_t id, krypto::utils::timestamp_t timestamp,
                             const std::vector<std::tuple<krypto::utils::price_t,
                                     krypto::utils::quantity_t, krypto::serialization::OrderSide>> &);
    };

}