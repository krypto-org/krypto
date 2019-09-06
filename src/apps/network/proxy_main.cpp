#include <iostream>
#include <csignal>
#include <future>

#include <krypto/network/proxy.h>
#include <krypto/logger.h>


namespace {
    volatile std::sig_atomic_t SIGNAL_STATUS;
}

std::function<void(int)> shutdown_handler;

void signal_handler(int signal) { shutdown_handler(signal); }

int main(int argc, char ** argv) {
    if (argc <  3) {
        KRYP_LOG(error, "Provide config file as parameter: {} <config>", argv[0]);
        return 1;
    }

    const krypto::Config config(argv[1]);

    krypto::network::ZmqProxy proxy{config, argv[2]};
    auto done = std::async(std::launch::async, [&proxy] () {proxy.start();});

    shutdown_handler = [&proxy](int signal) {
        SIGNAL_STATUS = signal;
        proxy.stop();
    };

    std::signal(SIGINT, signal_handler);

    done.wait();

    KRYP_LOG(info, "Shutdown Proxy");
    return 0;
}




