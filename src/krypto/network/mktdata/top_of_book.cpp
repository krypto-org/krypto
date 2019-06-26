#include <krypto/network/mktdata/top_of_book.h>
#include <krypto/mktdata/convert.h>
#include <krypto/logger.h>


void krypto::network::mktdata::TopOfBookPublisher::serialize(
        const krypto::mktdata::Quote &quote) {
    krypto::serialization::QuoteBuilder quote_builder{fb_builder_};

    quote_builder.add_timestamp(quote.timestamp);
    quote_builder.add_security_id(quote.security_id);
    quote_builder.add_bid(quote.bid);
    quote_builder.add_ask(quote.ask);
    quote_builder.add_last(quote.last);
    quote_builder.add_bid_quantity(quote.bid_qty);
    quote_builder.add_ask_quantity(quote.ask_qty);
    quote_builder.add_last_quantity(quote.last_qty);

    auto q = quote_builder.Finish();
    fb_builder_.Finish(q);
}

void krypto::network::mktdata::TopOfBookPublisher::serialize(const krypto::mktdata::Trade &trade) {
    krypto::serialization::TradeBuilder trade_builder{fb_builder_};

    trade_builder.add_timestamp(trade.timestamp);
    trade_builder.add_security_id(trade.security_id);
    trade_builder.add_price(trade.price);
    trade_builder.add_quantity(trade.quantity);
    trade_builder.add_side(
            krypto::serialization::EnumValuesSide()[
                    typename std::underlying_type<krypto::mktdata::Side>::type(
                            trade.side)]);
    auto t = trade_builder.Finish();
    fb_builder_.Finish(t);
}

