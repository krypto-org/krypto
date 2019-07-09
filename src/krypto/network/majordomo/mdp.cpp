#include <krypto/network/majordomo/mdp.h>

namespace krypto::network::mdp {

    std::string generate_id() {
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
        return std::to_string(nanoseconds.count());
    }

}
