#include <iostream>

#include <krypto/config.h>
#include <krypto/logger.h>
#include <krypto/exchanges/coinbase/api.h>
#include <gconv.h>

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
    krypto::exchanges::coinbase::Api api{config, "sandbox", authenticator};
    KRYP_LOG(info, "Time: {}", api.get_time().value().dump());
    std::string acct = "72acfba5-a3be-40f9-94e6-881732525ccc";
    KRYP_LOG(info, "Products: {}", api.get_products().value());
    KRYP_LOG(info, "Balances: {}", api.get_balances().value());
    KRYP_LOG(info, "Balances: {}", api.get_balances(acct).value());
    KRYP_LOG(info, "Balances: {}", api.get_balances_history(acct).value());

    krypto::exchanges::coinbase::Order order{
            "BTC-USD", "buy", 0.01, 1.0, "limit"};
    std::optional<std::string> order_response = api.send_limit_order(order);
    if (order_response.has_value()) {
        KRYP_LOG(info, "Order Response: {}", order_response.value());
    }
}




