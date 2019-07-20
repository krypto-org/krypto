#include <krypto/utils/common.h>
#include <krypto/instruments/client.h>

int main() {
    krypto::utils::Startup::init();
    zmq::context_t context(1);
    krypto::instruments::InstrumentClient client{context, true ,"tcp://localhost:8686"};
    auto result = client.query_all();
    KRYP_LOG(info, "{}", result.size());
}
