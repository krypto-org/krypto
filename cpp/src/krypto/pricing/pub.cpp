#include <krypto/pricing/pub.h>
#include <krypto/utils/date_time.h>

void krypto::pricing::Publisher::serialize(
        const krypto::utils::TheoreticalSnapshot &snapshot) {
    krypto::serialization::TheoreticalSnapshotBuilder builder{fb_builder_};
    builder.add_timestamp(krypto::utils::current_time_in_nanoseconds());
    builder.add_security_id(snapshot.security_id);
    builder.add_price(snapshot.price);
    builder.add_error(snapshot.error);
    auto theo_offset = builder.Finish();
    fb_builder_.Finish(theo_offset);
}
