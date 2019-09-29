#include <iostream>
#include <csignal>
#include <future>

#include <krypto/utils/id_gen.h>

#include <krypto/utils/common.h>
#include <krypto/orders/client.h>
#include <krypto/mktdata/convert.h>
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

    krypto::orders::OrderClient client{context, config};
    auto done = std::async(std::launch::async, [&client]() {
        client.subscribe(krypto::utils::MsgType::UNDEFINED);
        client.start();
    });

    std::this_thread::sleep_for(std::chrono::seconds{1});

    shutdown_handler = [&client](int signal) {
        SIGNAL_STATUS = signal;
        client.stop();
    };

    std::atomic_bool send_cancel = true;

    client.register_listener([&] (auto&& update) {
        KRYP_LOG(info, "Received Order Update: {}", update->order_id()->str());
        if (update->status() == krypto::serialization::OrderStatus_IN_FLIGHT) {
            KRYP_LOG(info, "{} In Flight", update->order_id()->str());
        } else if (update->status() == krypto::serialization::OrderStatus_FILLED) {
            KRYP_LOG(info, "{} Filled", update->order_id()->str());
            send_cancel = false;
        } else if (update->status() == krypto::serialization::OrderStatus_ACCEPTED) {
            KRYP_LOG(info, "{} Accepted", update->order_id()->str());
        } else if (update->status() == krypto::serialization::OrderStatus_REJECTED) {
            KRYP_LOG(info, "{} Rejected", update->order_id()->str());
            send_cancel = false;
        }
    });

    std::signal(SIGINT, signal_handler);

    int64_t security_id = 10100030018;
    int64_t price = krypto::mktdata::convert_price(12000.0);
    int64_t qty = krypto::mktdata::convert_price(0.01);

    auto order_id = client.new_order("sim-orders",
                     security_id,
                     price,
                     qty,
                     krypto::serialization::Side::Side_BUY,
                     krypto::serialization::TimeInForce_IOC);

    KRYP_LOG(info, "Order Id: {}", order_id);

    std::this_thread::sleep_for(std::chrono::seconds{5});

    if (send_cancel) {
        client.cancel_order("sim-orders", order_id);
    }

    std::this_thread::sleep_for(std::chrono::seconds{2});

    done.wait();

    KRYP_LOG(info, "Shutdown Client");
    return 0;
}