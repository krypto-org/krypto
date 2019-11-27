#include <iostream>
#include <csignal>
#include <future>

#include <krypto/config.h>
#include <krypto/logger.h>
#include <krypto/utils/common.h>

#include <krypto/orders/sim/server.h>
#include <krypto/utils/types.h>

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

    using order_server_t = krypto::orders::sim::OrderServer<
            krypto::serialization::Exchange::Exchange_COINBASE, true>;
    order_server_t orders_server{context, config};

    std::thread server_thread(
            std::bind(&order_server_t::start, &orders_server));

    shutdown_handler = [&](int signal) {
        SIGNAL_STATUS = signal;
        orders_server.stop();
    };

    std::signal(SIGINT, signal_handler);

    KRYP_LOG(info, "Joining server thread");
    server_thread.join();

    KRYP_LOG(info, "Shutdown Server");
    return 0;
}


