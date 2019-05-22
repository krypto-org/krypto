#include "krypto/network/mktdata/trade.h"

void TradePublisher::generate_buffer(const krypto::mktdata::Trade &trade) {
    krypto::serialization::TradeBuilder trade_builder{fb_builder_};

    trade_builder.add_timestamp(trade.timestamp);
    trade_builder.add_security_id(trade.security_id);
    trade_builder.add_price(trade.price);
    trade_builder.add_quantity(trade.quantity);
    trade_builder.add_side(
            krypto::serialization::EnumValuesSide()[
                    typename std::underlying_type<krypto::mktdata::Side>::type(
                            trade.side)]);
    trade_builder.add_timestamp(trade.timestamp);

    auto t = trade_builder.Finish();
    fb_builder_.Finish(t);
}
