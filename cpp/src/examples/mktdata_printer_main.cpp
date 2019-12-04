#include <iostream>
#include <csignal>
#include <future>

#include <krypto/config.h>
#include <krypto/logger.h>
#include <krypto/utils/common.h>
#include <krypto/network/subscriber.h>
#include <krypto/mktdata/protocol.h>
#include <krypto/utils/types.h>

namespace {
    struct MktdataPrinter : public krypto::network::Subscriber<MktdataPrinter, krypto::mktdata::Parser, true> {
        using krypto::network::Subscriber<MktdataPrinter, krypto::mktdata::Parser, true>::Subscriber;

        void process(const krypto::serialization::Quote *quote) {
//            std::cout << "QUOTE: " << quote->security_id() << '\n';
        }

        void process(const krypto::serialization::Trade *trade) {
//            std::cout << "TRADE: " << trade->security_id() << '\n';
        }

        void process(const krypto::serialization::Heartbeat *hb) {
//            std::cout << "HB: " << hb->security_id() << '\n';
        }

        void process(const krypto::serialization::OrderUpdate *ou) {
            KRYP_LOG(info, "Order Update: {} | {} | {} | {}",
                     ou->timestamp(),
                     ou->exchange_order_id()->str(),
                     krypto::serialization::EnumNameOrderStatus(ou->status()),
                     ou->filled_quantity()
            );
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

    const krypto::Config config(argv[1]);

    zmq::context_t context{1};

    MktdataPrinter printer{context, config.at<std::string>("/services/mktdata_gateway/backend/client")};
    auto done = std::async(std::launch::async, [&printer]() {
        printer.subscribe(krypto::utils::MsgType::UNDEFINED);
        printer.start();
    });

    shutdown_handler = [&printer](int signal) {
        SIGNAL_STATUS = signal;
        printer.stop();
    };

    std::signal(SIGINT, signal_handler);

    done.wait();

    KRYP_LOG(info, "Shutdown Server");
    return 0;
}


