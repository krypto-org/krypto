#include <iostream>
#include <csignal>
#include <future>

#include <krypto/mktdata/gateway.h>
#include <krypto/logger.h>


namespace {
    volatile std::sig_atomic_t SIGNAL_STATUS;
}

std::function<void(int)> shutdown_handler;

void signal_handler(int signal) { shutdown_handler(signal); }

int main(int argc, char ** argv) {
    if (argc <  2) {
        KRYP_LOG(error, "Provide config file as parameter: {} <config>", argv[0]);
        return 1;
    }

    zmq::context_t context{1};
    const krypto::Config config(argv[1]);

    krypto::mktdata::MktdataGateway<true> mktdata_gateway{context, config};
    auto done = std::async(std::launch::async, [&mktdata_gateway] () {mktdata_gateway.start();});

    shutdown_handler = [&mktdata_gateway](int signal) {
        SIGNAL_STATUS = signal;
        mktdata_gateway.stop();
    };

    std::signal(SIGINT, signal_handler);

    done.wait();

    KRYP_LOG(info, "Shutdown Broker");
    return 0;
}




