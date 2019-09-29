#include <utility>

#include <krypto/instruments/server.h>
#include <krypto/utils/enum.h>
#include <krypto/utils/types.h>


krypto::utils::MsgType krypto::instruments::Server::process(const krypto::serialization::InstrumentRequest *) {
    fb_builder_.Clear();
    std::vector<flatbuffers::Offset<krypto::serialization::Instrument>> instruments;
    for (auto &&inst : cache_) {
        auto inst_offset = krypto::serialization::CreateInstrument(
                fb_builder_, inst.id, inst.inst_type,
                fb_builder_.CreateString(inst.symbol),
                inst.exchange,
                fb_builder_.CreateString(inst.exchange_symbol),
                inst.tick_size,
                inst.min_size,
                inst.max_size,
                inst.crypto_base,
                inst.crypto_quote);
        instruments.emplace_back(inst_offset);
    }
    auto instruments_offset = fb_builder_.CreateVector(instruments);
    krypto::serialization::InstrumentResponseBuilder builder{fb_builder_};
    builder.add_instruments(instruments_offset);
    auto response = builder.Finish();
    fb_builder_.Finish(response);

    return krypto::utils::MsgType::INSTRUMENT_RESPONSE;
}

krypto::instruments::Server::Server(zmq::context_t &context, const krypto::Config &config, std::string service) :
        WorkerBase(context, config, std::move(service)), store_{config} {
    cache_ = store_.load();
}

krypto::utils::MsgType
krypto::instruments::Server::process(const krypto::serialization::InstrumentCacheRefreshRequest *request) {
    cache_.clear();
    cache_ = store_.load();
    return krypto::utils::MsgType::NO_PAYLOAD;
}
