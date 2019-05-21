#include <krypto/network/mktdata/quote.h>
#include <krypto/mktdata/convert.h>

void krypto::network::mktdata::QuotePublisher::generate_buffer(
        const krypto::mktdata::Quote &quote) {
    krypto::serialization::QuoteBuilder quote_builder{fb_builder_};

    quote_builder.add_timestamp(quote.timestamp);
    quote_builder.add_security_id(quote.security_id);
    quote_builder.add_bid(krypto::mktdata::convert_price(quote.bid));
    quote_builder.add_ask(krypto::mktdata::convert_price(quote.ask));
    quote_builder.add_last(krypto::mktdata::convert_price(quote.last));
    quote_builder.add_bid_quantity(krypto::mktdata::convert_quantity(quote.bid_qty));
    quote_builder.add_ask_quantity(krypto::mktdata::convert_quantity(quote.ask_qty));
    quote_builder.add_last_quantity(krypto::mktdata::convert_quantity(quote.last_qty));

    auto q = quote_builder.Finish();

    fb_builder_.Finish(q);
}

