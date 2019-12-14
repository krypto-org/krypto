#include <krypto/mktdata/pub.h>
#include <krypto/mktdata/convert.h>
#include <krypto/utils/types.h>
#include <krypto/serialization/helper.h>
#include <krypto/pricing/pub.h>


void krypto::mktdata::Publisher::serialize(
        const krypto::utils::Quote &quote) {
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

void krypto::mktdata::Publisher::serialize(const krypto::utils::Trade &trade) {
    auto tid_offset = fb_builder_.CreateString(trade.trade_id);
    krypto::serialization::TradeBuilder trade_builder{fb_builder_};
    trade_builder.add_trade_id(tid_offset);
    trade_builder.add_timestamp(trade.timestamp);
    trade_builder.add_security_id(trade.security_id);
    trade_builder.add_price(trade.price);
    trade_builder.add_quantity(trade.quantity);
    trade_builder.add_side(
            krypto::serialization::EnumValuesSide()[
                    typename std::underlying_type<krypto::utils::Side>::type(
                            trade.side)]);
    auto t = trade_builder.Finish();
    fb_builder_.Finish(t);
}

void krypto::mktdata::Publisher::serialize(const krypto::utils::Heartbeat &hb) {
    krypto::serialization::HeartbeatBuilder hb_builder{fb_builder_};
    hb_builder.add_security_id(hb.security_id);
    hb_builder.add_timestamp(hb.timestamp);
    auto hb_offset = hb_builder.Finish();
    fb_builder_.Finish(hb_offset);
}

void krypto::mktdata::Publisher::serialize(const krypto::utils::OrderUpdate& order_update) {
    krypto::serialization::serialize(fb_builder_, order_update);
}
