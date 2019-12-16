#include <krypto/utils/common.h>
#include <krypto/instruments/client.h>

int main(int argc, char ** argv) {
    if (argc <  2) {
        KRYP_LOG(error, "Provide config file as parameter: {} <config>", argv[0]);
        return 1;
    }

    const krypto::Config config(argv[1]);

    zmq::context_t context{1};

    krypto::instruments::InstrumentClient client{context, config};
    
    auto result = client.query();
    KRYP_LOG(info, "Sent query waiting for result");
    KRYP_LOG(info, "Received {} instruments", result.get().size());
    KRYP_LOG(info, "Refresh Cache");
    client.refresh_cache();
    auto insts = client.query().get();
    KRYP_LOG(info, "{}", insts.size());
    for (auto&& inst: insts) {
        KRYP_LOG(info, "{} : {} => {}", inst.symbol, inst.id, inst.active);
    }
}
