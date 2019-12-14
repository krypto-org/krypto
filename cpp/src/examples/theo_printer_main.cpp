#include <iostream>
#include <csignal>
#include <future>

#include <krypto/config.h>
#include <krypto/logger.h>
#include <krypto/pricing/client.h>
#include <krypto/utils/types.h>

std::function<void(int)> shutdown_handler;

namespace {
    volatile std::sig_atomic_t SIGNAL_STATUS;
}

void signal_handler(int signal) { shutdown_handler(signal); }


int main(int argc, char **argv) {
    if (argc < 2) {
        KRYP_LOG(error, "Provide config file as parameter: {} <config>", argv[0]);
        return 1;
    }

    const krypto::Config config(argv[1]);

    zmq::context_t context{1};

    krypto::pricing::PricingClient client{
        context,
        config.at<std::string>("/services/pricing/client")};
    client.register_listener([] (auto&& snapshot) {
        KRYP_LOG(info, "{}: {} +/- {}", snapshot->security_id(),
                snapshot->price(), snapshot->error());
    });

    auto done = std::async(std::launch::async, [&client]() {
        client.subscribe(krypto::utils::MsgType::UNDEFINED);
        client.start();
    });

    shutdown_handler = [&client](int signal) {
        SIGNAL_STATUS = signal;
        client.stop();
    };

    std::signal(SIGINT, signal_handler);

    done.wait();

    KRYP_LOG(info, "Shutdown Server");
    return 0;
}



