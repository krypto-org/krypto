#include <iostream>
#include <csignal>
#include <future>

#include <krypto/config.h>
#include <krypto/logger.h>
#include <krypto/utils/common.h>
//#include <krypto/mktdata/client.h>
#include <krypto/mktdata/ws_server.h>
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

    krypto::mktdata::WebsocketServer ws{context, config};

    std::thread mktdata_thread(std::bind(&krypto::mktdata::WebsocketServer::start, &ws));
    ws.subscribe(krypto::utils::MsgType::UNDEFINED);

    std::thread ws_thread(std::bind(&krypto::mktdata::WebsocketServer::process_messages, &ws));

    ws.ioc_run();

    shutdown_handler = [&ws](int signal) {
        SIGNAL_STATUS = signal;
        ws.done();
        ws.stop();
    };

    std::signal(SIGINT, signal_handler);

    ws_thread.join();
    mktdata_thread.join();

    KRYP_LOG(info, "Shutdown Server");
    return 0;
}


