#include <krypto/common/startup.h>
#include <krypto/instruments/client.h>

int main() {
    krypto::common::Startup::init();
    zmq::context_t context(1);
    krypto::instruments::InstrumentClient client{context, true ,"tcp://localhost:5555"};
    auto result = client.query_all();
    KRYP_LOG(info, "{}", result.size());
}
