#include <krypto/utils/common.h>
#include <krypto/instruments/client.h>

int main(int argc, char ** argv) {
    krypto::utils::Startup::init();

    if (argc <  2) {
        KRYP_LOG(error, "Provide config file as parameter: {} <config>", argv[0]);
        return 1;
    }

    const krypto::Config config(argv[1]);

    zmq::context_t context{1};

    krypto::instruments::InstrumentClient client{context, config};
    
    auto result = client.query_all();
    KRYP_LOG(info, "{}", result.size());
    KRYP_LOG(info, "Refresh Cache");
    client.refresh_cache();
    result = client.query_all();
    KRYP_LOG(info, "{}", result.size());
    for (auto&& inst: result) {
        KRYP_LOG(info, "{} : {}", inst.symbol, inst.id);
    }
}
