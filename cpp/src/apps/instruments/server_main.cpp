#include <iostream>
#include <csignal>
#include <future>

#include <krypto/network/rpc/broker.h>
#include <krypto/config.h>
#include <krypto/logger.h>
#include <krypto/instruments/server.h>


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

    krypto::instruments::Server<true> server{context, config};
    auto done = std::async(std::launch::async, [&server]() { server.start(); });

    shutdown_handler = [&server](int signal) {
        SIGNAL_STATUS = signal;
        server.stop();
    };

    std::signal(SIGINT, signal_handler);

    done.wait();

    KRYP_LOG(info, "Shutdown Server");
    return 0;
}

