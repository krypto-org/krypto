#pragma once;

#include <krypto/network/publisher.h>
#include <krypto/mktdata/book.h>

class TradePublisher final : public krypto::network::PublisherBase<TradePublisher, krypto::mktdata::Trade> {

public:
    using krypto::network::PublisherBase<TradePublisher, krypto::mktdata::Trade>::PublisherBase;

    using krypto::network::PublisherBase<TradePublisher, krypto::mktdata::Trade>::send;

    void generate_buffer(const krypto::mktdata::Trade& trade);
};
