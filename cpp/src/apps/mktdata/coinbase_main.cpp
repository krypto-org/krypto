#include <iostream>
#include <string>

#include <krypto/logger.h>
#include <krypto/mktdata/coinbase/server.h>

namespace coinbase = krypto::mktdata::coinbase;

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

    krypto::mktdata::coinbase::Server<true> server{context, config};

    shutdown_handler = [&server](int signal) {
        SIGNAL_STATUS = signal;
        server.stop();
    };
    std::signal(SIGINT, signal_handler);

    server.start();

    KRYP_LOG(info, "Closed Client Connection. Exiting");
    return 0;
}