#include <iostream>
#include <csignal>
#include <future>

#include <krypto/config.h>
#include <krypto/logger.h>
#include <krypto/utils/common.h>

#include <krypto/orders/coinbase//server.h>
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

    std::string key_file{std::getenv("CB_KEY_FILE")};
    krypto::exchanges::coinbase::Authenticator authenticator =
            krypto::exchanges::coinbase::Authenticator::from_file(key_file);
    krypto::exchanges::coinbase::Api api{config, "sandbox", authenticator};

    using order_server_t = krypto::orders::coinbase::OrderServer<true>;
    order_server_t orders_server{context, api, config, true};

    std::thread server_thread(
            [ObjectPtr = &orders_server] { ObjectPtr->start(); });

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



