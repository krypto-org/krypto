#include <iostream>
#include <csignal>
#include <thread>

#include <krypto/network/rpc/broker.h>
#include <krypto/logger.h>


namespace {
    volatile std::sig_atomic_t SIGNAL_STATUS;
}

std::function<void(int)> shutdown_handler;

void signal_handler(int signal) { shutdown_handler(signal); }

int main() {
    zmq::context_t context(1);
    krypto::network::rpc::Broker broker{context, "tcp://*:5555", "tcp://*:5556"};

    std::thread broker_thread(std::bind(&krypto::network::rpc::Broker::start, &broker));

    shutdown_handler = [&broker](int signal) {
        SIGNAL_STATUS = signal;
        broker.stop();
    };
    std::signal(SIGINT, signal_handler);

    broker_thread.join();

    KRYP_LOG(info, "Shutdown Broker");
    return 0;
}




