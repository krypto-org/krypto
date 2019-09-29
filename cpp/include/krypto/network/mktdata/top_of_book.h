#pragma once

#include <krypto/network/publisher.h>
#include <krypto/utils/common.h>
#include <krypto/mktdata/protocol.h>
#include <krypto/utils/types.h>


namespace krypto::network::mktdata {

    class TopOfBookPublisher : public krypto::network::PublisherBase<TopOfBookPublisher> {

    public:
        using krypto::network::PublisherBase<TopOfBookPublisher>::PublisherBase;

        using krypto::network::PublisherBase<TopOfBookPublisher>::send;

        using krypto::network::PublisherBase<TopOfBookPublisher>::connect;

        void serialize(const krypto::utils::Quote& quote);

        void serialize(const krypto::utils::Trade& quote);

        void serialize(const krypto::utils::Heartbeat& hb);
    };

}
