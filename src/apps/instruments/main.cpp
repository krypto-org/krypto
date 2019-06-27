#include <krypto/config.h>
#include <krypto/instruments/store.h>
#include <krypto/logger.h>
#include <krypto/common/startup.h>

int main(int argc, char **argv) {
    krypto::common::Startup::init();
    const krypto::Config config(argv[1]);
    krypto::instruments::InstrumentStore store{config};
    auto v = store.load();
    KRYP_LOG(info, "Loaded {} Instruments", v.size());
}