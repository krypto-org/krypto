#include <iostream>
#include <csignal>
#include <thread>

#include <krypto/network/rpc/broker.h>
#include <krypto/config.h>
#include <krypto/instruments/store.h>
#include <krypto/logger.h>
#include <krypto/common/startup.h>
#include <krypto/instruments/server.h>


namespace {
    volatile std::sig_atomic_t SIGNAL_STATUS;
}

std::function<void(int)> shutdown_handler;

void signal_handler(int signal) { shutdown_handler(signal); }

int main() {
    krypto::common::Startup::init();

    zmq::context_t context(1);
    krypto::instruments::Server server{context, "tcp://127.0.0.1:5556", "instruments", true};

    std::thread broker_thread(std::bind(&krypto::instruments::Server::start, &server));

    shutdown_handler = [&server](int signal) {
        SIGNAL_STATUS = signal;
        server.stop();
    };
    std::signal(SIGINT, signal_handler);

    broker_thread.join();

    KRYP_LOG(info, "Shutdown Server");
    return 0;
}

