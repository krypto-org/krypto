#include <iostream>
#include <csignal>
#include <future>

#include <krypto/config.h>
#include <krypto/logger.h>
#include <krypto/utils/common.h>

#include <krypto/orders/sim/server.h>

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

    krypto::utils::Startup::init();
    const krypto::Config config(argv[1]);

    zmq::context_t context{1};

    krypto::orders::sim::OrderServer orders_server{context, config, "sim-orders"};
    krypto::orders::sim::MktdataSubscriber mktdata_subscriber(context, config, orders_server);

    mktdata_subscriber.subscribe(krypto::utils::MsgType::UNDEFINED);

    std::thread server_thread(
            std::bind(&krypto::orders::sim::OrderServer::start,
                    &orders_server));

    std::thread mktdata_thread(
            std::bind(&krypto::orders::sim::MktdataSubscriber::start,
                    &mktdata_subscriber));
    mktdata_subscriber.subscribe(krypto::utils::MsgType::UNDEFINED);

    shutdown_handler = [&](int signal) {
        SIGNAL_STATUS = signal;
        mktdata_subscriber.stop();
        orders_server.stop();
    };

    std::signal(SIGINT, signal_handler);

    KRYP_LOG(info, "Joining md server thread");
    mktdata_thread.join();

    KRYP_LOG(info, "Joining server thread");
    server_thread.join();

    KRYP_LOG(info, "Shutdown Server");
    return 0;
}


