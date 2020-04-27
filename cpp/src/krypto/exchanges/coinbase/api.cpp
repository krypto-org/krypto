#include <krypto/exchanges/coinbase/api.h>

#include <utility>

namespace krypto::exchanges::coinbase {
    Api::Api(const krypto::Config &config, const std::string &environment,
             krypto::exchanges::coinbase::Authenticator authenticator) :
            http_client_{
                    config.at<std::string>("/exchanges/coinbase/rest/base_url/" + environment)},
            inst_endpoint_{"/" + config.at<std::string>(
                    "/exchanges/coinbase/rest/channels/instruments")},
            acct_endpoint_{"/" + config.at<std::string>(
                    "/exchanges/coinbase/rest/channels/accounts")},
            orders_endpoint_{"/" + config.at<std::string>(
                    "/exchanges/coinbase/rest/channels/orders")},
            authenticator_(std::move(authenticator)) {}

    std::string Api::create_order_message(const Order &order) {
        nlohmann::json order_json;
        order_json["product_id"] = order.product_id;
        order_json["side"] = order.side;
        order_json["price"] = order.price;
        order_json["size"] = order.size;
        order_json["type"] = order.order_type;
        return order_json.dump();
    }

    std::optional<std::string> Api::get_products() {
        return http_client_.get(inst_endpoint_, {});
    }

    std::optional<std::string> Api::get_balances() {
        return get_balances("");
    }

    std::optional<std::string> Api::get_balances(const std::string &account) {
        std::unordered_map<std::string, std::string> headers;
        std::string ts = krypto::exchanges::coinbase::Authenticator::get_timestamp();
        std::string endpoint = "/accounts" + (account.empty() ? "" : "/" + account);
        std::optional<std::string> sign = authenticator_.sign(
                ts, "GET", endpoint, "");
        if (!sign.has_value()) {
            return std::nullopt;
        }
        headers["CB-ACCESS-KEY"] = authenticator_.key();
        headers["CB-ACCESS-PASSPHRASE"] = authenticator_.passphrase();
        headers["CB-ACCESS-SIGN"] = sign.value();
        headers["CB-ACCESS-TIMESTAMP"] = ts;
        return http_client_.get(endpoint, headers);
    }

    std::optional<std::string> Api::get_balances_history(const std::string &account) {
        return get_balances(account + "/ledger");
    }

    std::optional<std::string> Api::send_limit_order(const Order &order) {
        std::unordered_map<std::string, std::string> headers;
        std::optional<nlohmann::json> server_time = get_time();
        if (!server_time.has_value()) {
            return std::nullopt;
        }
        std::string ts = Authenticator::get_timestamp();
        std::string order_str = create_order_message(order);
        std::optional<std::string> sign = authenticator_.sign(
                ts, "POST", orders_endpoint_, order_str);

        if (!sign.has_value()) {
            return std::nullopt;
        }

        KRYP_LOG(info, "{} | {} | {} | {}",
                 authenticator_.key(),
                 authenticator_.passphrase(), sign.value(), ts);

        headers["CB-ACCESS-KEY"] = authenticator_.key();
        headers["CB-ACCESS-PASSPHRASE"] = authenticator_.passphrase();
        headers["CB-ACCESS-SIGN"] = sign.value();
        headers["CB-ACCESS-TIMESTAMP"] = ts;
        auto response = http_client_.post(orders_endpoint_, order_str, headers);
        if (!response.has_value()) {
            KRYP_LOG(error, "No response");
        }
        return response;
    }

    std::optional<nlohmann::json> Api::get_time() {
        return nlohmann::json::parse(http_client_.get("/time", {}).value());
    }
}