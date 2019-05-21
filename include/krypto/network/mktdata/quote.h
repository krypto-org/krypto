#pragma once

#include <krypto/network/publisher.h>
#include <krypto/mktdata/book.h>

namespace krypto::network::mktdata {

    class QuotePublisher : public krypto::network::PublisherBase<QuotePublisher, krypto::mktdata::Quote> {
        using krypto::network::PublisherBase<QuotePublisher, krypto::mktdata::Quote>::PublisherBase;

        using krypto::network::PublisherBase<QuotePublisher, krypto::mktdata::Quote>::send;

        void generate_buffer(const krypto::mktdata::Quote& quote);

    };

}
