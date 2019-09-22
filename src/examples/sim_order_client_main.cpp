#include <iostream>
#include <csignal>
#include <future>

#include <krypto/utils/id_gen.h>

#include <krypto/utils/common.h>
#include <krypto/orders/client.h>

namespace {
    struct Client : public krypto::orders::OrderClient<Client> {
        using krypto::orders::OrderClient<Client>::OrderClient;
    };

    volatile std::sig_atomic_t SIGNAL_STATUS;
}

std::function<void(int)> shutdown_handler;

void signal_handler(int signal) { shutdown_handler(signal); }

int main(int argc, char **argv) {
    krypto::utils::Startup::init();

    if (argc < 2) {
        KRYP_LOG(error, "Provide config file as parameter: {} <config>", argv[0]);
        return 1;
    }

    const krypto::Config config(argv[1]);

    zmq::context_t context{1};

    Client client{context, config};
    auto done = std::async(std::launch::async, [&client]() {
        client.subscribe(krypto::utils::MsgType::UNDEFINED);
        client.start();
    });

    shutdown_handler = [&client](int signal) {
        SIGNAL_STATUS = signal;
        client.stop();
    };

    std::signal(SIGINT, signal_handler);

    client.new_order("sim-orders",
                     1,
                     1230982312,
                     1239871,
                     krypto::serialization::Side::Side_BUY,
                     krypto::serialization::TimeInForce_DAY);

    client.cancel_order("sim-orders", "sim_order");

    client.replace_order("sim-orders", "sim_order", 1230982312,
                         1239871,
                         krypto::serialization::Side::Side_BUY);

    done.wait();

    KRYP_LOG(info, "Shutdown Client");
    return 0;
}