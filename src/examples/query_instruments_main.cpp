#include <krypto/instruments/client.h>

int main() {
    zmq::context_t context(1);
    krypto::instruments::InstrumentClient client{context, true ,"tcp://localhost:5555"};
    auto result = client.query_all();
    KRYP_LOG(info, "{}", result.size());
}
