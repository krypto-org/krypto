#pragma once

#include <krypto/exchanges/coinbase/auth.h>
#include <krypto/utils/common.h>
#include <krypto/exchanges/base.h>
#include <krypto/http.h>
#include <nlohmann/json.hpp>
#include <krypto/config.h>
#include <krypto/exchanges/coinbase/types.h>

namespace krypto::exchanges::coinbase {
    class Api final {
    private:
        krypto::HttpClient http_client_;
        std::string inst_endpoint_;
        std::string acct_endpoint_;
        std::string orders_endpoint_;
        Authenticator authenticator_;
        static std::string create_order_message(const Order& order);
    public:
        Api(const krypto::Config &config, const std::string& environment,
                Authenticator  authenticator);
        std::optional<nlohmann::json> get_time();
        std::optional<std::string> get_products();
        std::optional<std::string> get_balances();
        std::optional<std::string> get_balances(const std::string& account);
        std::optional<std::string> get_balances_history(const std::string& account);
        std::optional<std::string> send_limit_order(const Order& order);
    };
}