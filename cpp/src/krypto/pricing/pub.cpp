#include <krypto/pricing/pub.h>
#include <krypto/utils/date_time.h>

void krypto::pricing::Publisher::serialize(
        const krypto::utils::TheoreticalSnapshot &snapshot) {
    krypto::serialization::TheoreticalSnapshotBuilder builder{fb_builder_};
    builder.add_timestamp(krypto::utils::current_time_in_nanoseconds());
    builder.add_security_id(snapshot.security_id);
    builder.add_price(snapshot.price);
    builder.add_adjusted_price(snapshot.adjusted_price);
    builder.add_error(snapshot.error);
    builder.add_mm_base_bid(snapshot.mm_base_bid);
    builder.add_mm_base_ask(snapshot.mm_base_ask);
    builder.add_mm_base_spread(snapshot.mm_base_spread);
    builder.add_bid_liquidity(snapshot.bid_liquidity);
    builder.add_ask_liquidity(snapshot.ask_liquidity);
    auto theo_offset = builder.Finish();
    fb_builder_.Finish(theo_offset);
}
