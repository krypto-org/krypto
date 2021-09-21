#include <csignal>

#include <krypto/logger.h>
#include <krypto/mktdata/redis_publisher.h>

namespace {
    volatile std::sig_atomic_t SIGNAL_STATUS;
}

std::function<void(int)> shutdown_handler;

void signal_handler(int signal) { shutdown_handler(signal); }

int main(int argc, char **argv) {
    if (argc < 2) {
        KRYP_LOG(error, "Provide config file as parameter: {} <config>", argv[0]);
        return 1;
    }

    const krypto::Config config(argv[1]);

    zmq::context_t context{1};

    krypto::mktdata::RedisPublisher cache{context, config};

    shutdown_handler = [&cache](int signal) {
        SIGNAL_STATUS = signal;
        cache.stop();
    };
    std::signal(SIGINT, signal_handler);

    cache.start();

    KRYP_LOG(info, "Closed Client Connection. Exiting");
    return 0;
}