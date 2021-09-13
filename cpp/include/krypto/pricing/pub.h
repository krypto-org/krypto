#pragma once

#include <krypto/network/publisher.h>
#include <krypto/utils/types.h>

namespace krypto::pricing {
    class Publisher final : public krypto::network::PublisherBase<Publisher> {
    public:
        using krypto::network::PublisherBase<Publisher>::PublisherBase;

        using krypto::network::PublisherBase<Publisher>::send;

        using krypto::network::PublisherBase<Publisher>::connect;

        void serialize(const krypto::utils::TheoreticalSnapshot& snapshot);
    };
}