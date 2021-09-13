#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include <krypto/logger.h>
#include <krypto/mktdata/coinbase/server.h>

namespace coinbase = krypto::mktdata::coinbase;
namespace po = boost::program_options;

namespace {
    volatile std::sig_atomic_t SIGNAL_STATUS;
}

std::function<void(int)> shutdown_handler;

void signal_handler(int signal) { shutdown_handler(signal); }

int main(int argc, char **argv) {
    po::options_description desc("CoinbaseInstruments market data server");
    bool orders_pnly = false;
    bool run_sandbox = false;
    desc.add_options()
            ("help", "produce help message")
            ("config", po::value<std::string>(), "path for config file")
            ("sandbox", po::bool_switch(&run_sandbox), "set environment (production is default)")
            ("orders-only", po::bool_switch(&orders_pnly), "listen to order feed only");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }

    std::string config_path;

    if (vm.count("config")) {
        config_path = vm["config"].as<std::string>();
    } else {
        KRYP_LOG(error, "--config is required argument");
        return 1;
    }

    std::string env = "production";

    if (run_sandbox) {
        env = "sandbox";
        KRYP_LOG(info, "Starting in {} environment", env);
    }

    zmq::context_t context{1};

    const krypto::Config config(std::move(config_path));

    krypto::mktdata::coinbase::Server server{context, config, env, !orders_pnly};

    shutdown_handler = [&server](int signal) {
        SIGNAL_STATUS = signal;
        server.stop();
    };
    std::signal(SIGINT, signal_handler);

    server.start();

    KRYP_LOG(info, "Closed Client Connection. Exiting");
    return 0;
}