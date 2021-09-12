#pragma once

#include <krypto/exchanges/coinbase/auth.h>
#include <krypto/utils/common.h>
#include <krypto/instruments/exchanges/instrument.h>
#include <krypto/utils/http.h>
#include <nlohmann/json.hpp>
#include <krypto/config.h>
#include <krypto/exchanges/coinbase/types.h>

namespace krypto::exchanges::coinbase {

    class PublicApi {
    protected:
        krypto::utils::HttpClient http_client_;
        std::string inst_endpoint_;
    public:
        PublicApi(const krypto::Config &config, const std::string &environment);

        std::optional<nlohmann::json> get_time();

        std::optional<nlohmann::json> get_products();

    };

    class AuthenticatedApi final : public PublicApi {
    private:
        std::string acct_endpoint_;
        std::string orders_endpoint_;
        Authenticator authenticator_;

        static std::string create_order_message(const Order &order);

        std::optional<std::string> send_authenticated_request(
                const std::string &request_type, const std::string &endpoint, const std::string &data);

    public:
        AuthenticatedApi(const krypto::Config &config, const std::string &environment,
                         Authenticator authenticator);

        std::optional<nlohmann::json> get_accounts();

        std::optional<nlohmann::json> get_account(const std::string &account);

        std::optional<nlohmann::json> get_account_history(const std::string &account);

        std::optional<nlohmann::json> get_account_holds(const std::string &account);

        std::optional<nlohmann::json> place_order(const Order &order);

        std::optional<nlohmann::json> cancel_order(const std::string &order_id);

        std::optional<nlohmann::json> cancel_all(std::optional<std::string> product_id);

        std::optional<nlohmann::json> get_order(const std::optional<std::string> &status);

        std::optional<nlohmann::json> get_order(const std::string &product_id,
                                                const std::optional<std::string> &status);

        std::optional<nlohmann::json> get_fills_for_order(const std::string &order_id);

        std::optional<nlohmann::json> get_fills_for_product(const std::string &product_id);
    };
}