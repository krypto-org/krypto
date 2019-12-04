#pragma once

#include <krypto/network/publisher.h>
#include <krypto/utils/common.h>
#include <krypto/mktdata/protocol.h>
#include <krypto/utils/types.h>
#include <krypto/utils/message_types.h>


namespace krypto::mktdata {

    class Publisher : public krypto::network::PublisherBase<Publisher> {

    public:
        using krypto::network::PublisherBase<Publisher>::PublisherBase;

        using krypto::network::PublisherBase<Publisher>::send;

        using krypto::network::PublisherBase<Publisher>::connect;

        void serialize(const krypto::utils::Quote& quote);

        void serialize(const krypto::utils::Trade& quote);

        void serialize(const krypto::utils::OrderUpdate& order_update);

        void serialize(const krypto::utils::Heartbeat& hb);
    };

}
