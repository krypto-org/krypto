#pragma once

#include <krypto/network/publisher.h>
#include <krypto/mktdata/book.h>

namespace krypto::network::mktdata {

    class TopOfBookPublisher : public krypto::network::PublisherBase<TopOfBookPublisher> {

    public:
        using krypto::network::PublisherBase<TopOfBookPublisher>::PublisherBase;

        using krypto::network::PublisherBase<TopOfBookPublisher>::send;

        using krypto::network::PublisherBase<TopOfBookPublisher>::start;

        void serialize(const krypto::mktdata::Quote& quote);

        void serialize(const krypto::mktdata::Trade& quote);
    };

}
