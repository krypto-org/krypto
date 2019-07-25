#include <iostream>
#include <csignal>
#include <future>

#include <krypto/config.h>
#include <krypto/logger.h>
#include <krypto/utils/common.h>
#include <krypto/mktdata/client.h>


namespace {
    struct MktdataPrinter {
        static void consume(const krypto::serialization::Quote *quote) {
            std::cout << "QUOTE: "<< quote->security_id() << '\n';
        }

        static void consume(const krypto::serialization::Trade *trade) {
            std::cout << "TRADE: " << trade->security_id() << '\n';
        }
    };

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

    krypto::mktdata::Client<krypto::utils::ExchangeType::COINBASE,
            MktdataPrinter> client{config};
    auto done = std::async(std::launch::async, [&client]() {
        client.subscribe(krypto::utils::MsgType::ALL);
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


