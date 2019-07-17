#include <krypto/instruments/server.h>

void krypto::instruments::Server::process(const krypto::serialization::InstrumentRequest *) {
    krypto::serialization::InstrumentResponseBuilder builder{fb_builder_};
    auto resposne = builder.Finish();
    fb_builder_.Finish(resposne);
}
