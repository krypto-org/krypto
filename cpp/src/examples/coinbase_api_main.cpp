#include <iostream>

#include <krypto/config.h>
#include <krypto/logger.h>
#include <krypto/exchanges/coinbase/api.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        KRYP_LOG(error, "Provide config file as parameter: {} <config>", argv[0]);
        return 1;
    }
    const krypto::Config config(argv[1]);
    std::string key_file{std::getenv("CB_KEY_FILE")};
    KRYP_LOG(info, "{}", key_file);
    krypto::exchanges::coinbase::Authenticator authenticator =
            krypto::exchanges::coinbase::Authenticator::from_file(key_file);
    krypto::exchanges::coinbase::AuthenticatedApi api{config, "sandbox", authenticator};
    KRYP_LOG(info, "Time: {}", api.get_time().value().dump());
    std::string acct = "72acfba5-a3be-40f9-94e6-881732525ccc";
    KRYP_LOG(info, "Products: {}", api.get_products().value().dump());
    KRYP_LOG(info, "Balances: {}", api.get_accounts().value().dump());
    KRYP_LOG(info, "Balances: {}", api.get_account(acct).value().dump());
    KRYP_LOG(info, "Balances: {}", api.get_account_history(acct).value().dump());
    KRYP_LOG(info, "Holds: {}", api.get_account_holds(acct).value().dump());

    for (int price = 7700; price < 7710; ++price) {
        krypto::exchanges::coinbase::Order order{
                "BTC-USD", "buy", static_cast<double_t>(price), 0.01, "limit"};
        auto order_response = api.place_order(order);
        if (order_response.has_value()) {
            KRYP_LOG(info, "Order Response: {}", order_response.value().dump());
        }

        std::this_thread::sleep_for(std::chrono::milliseconds {200});
    }
    auto cancel_response = api.cancel_all("BTC-USD");
    if (cancel_response.has_value()) {
        KRYP_LOG(info, "Order Cancelled: {}", cancel_response.value().dump());
    }

    auto all_orders = api.get_order(std::nullopt);
    auto open_orders = api.get_order("open");
    auto pending_orders = api.get_order("pending");
    auto active_orders = api.get_order("active");

    if (all_orders.has_value()) {
        KRYP_LOG(info, "All Orders: {}", all_orders.value().dump());
    }

    if (open_orders.has_value()) {
        KRYP_LOG(info, "Open Orders: {}", open_orders.value().dump());
    }

    if (pending_orders.has_value()) {
        KRYP_LOG(info, "Pending Orders: {}", pending_orders.value().dump());
    }

    if (active_orders.has_value()) {
        KRYP_LOG(info, "Active Orders: {}", active_orders.value().dump());
    }

    auto open_btc_orders = api.get_order("BTC-USD", "open");
    if (open_btc_orders.has_value()) {
        KRYP_LOG(info, "Open BTC Orders: {}", open_btc_orders.value().dump());
    }

    auto btc_fills = api.get_fills_for_product("BTC-USD");
    if (btc_fills.has_value()) {
        KRYP_LOG(info, "BTC Fills: {}", btc_fills.value().dump());
    }

    auto order_fills = api.get_fills_for_order("e805bfd8-d7bc-4f6c-ab56-020003e0f608");
    if (order_fills.has_value()) {
        KRYP_LOG(info, "Order Fills: {}", order_fills.value().dump());
    }
}




