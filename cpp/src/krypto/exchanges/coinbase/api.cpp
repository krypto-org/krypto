#include <krypto/exchanges/coinbase/api.h>

#include <utility>

namespace krypto::exchanges::coinbase {
    AuthenticatedApi::AuthenticatedApi(const krypto::Config &config, const std::string &environment,
                                       krypto::exchanges::coinbase::Authenticator authenticator) :
            PublicApi{config, environment},
            account_endpoint_{"/" + config.at<std::string>(
                    "/exchanges/coinba  se/rest/channels/accounts")},
            orders_endpoint_{"/" + config.at<std::string>(
                    "/exchanges/coinbase/rest/channels/orders")},
            authenticator_(std::move(authenticator)) {}

    std::string AuthenticatedApi::create_order_message(const Order &order) {
        nlohmann::json order_json;
        order_json["product_id"] = order.product_id;
        order_json["side"] = order.side;
        order_json["price"] = order.price;
        order_json["size"] = order.size;
        order_json["type"] = order.order_type;
        order_json["post_only"] = order.post_only;
        if (order.client_oid.has_value()) {
            order_json["client_oid"] = order.client_oid.value();
        }
        if (order.self_trade_prevention_flag.has_value()) {
            order_json["stp"] = order.self_trade_prevention_flag.value();
        }
        if (order.stop.has_value() && order.stop_price.has_value()) {
            order_json["stop"] = order.stop.value();
            order_json["stop_price"] = order.stop_price.value();
        }
        if (order.tif.has_value()) {
            order_json["time_in_force"] = order.tif.value();
        }
        if (order.cancel_after.has_value()) {
            order_json["cancel_after"] = order.cancel_after.value();
        }
        return order_json.dump();
    }

    std::optional<std::string>
    AuthenticatedApi::send_authenticated_request(
            const std::string &request_type,
            const std::string &endpoint,
            const std::string &data) {
        std::unordered_map<std::string, std::string> headers;
        std::string ts = Authenticator::get_timestamp();
        std::optional<std::string> sign = authenticator_.sign(
                ts, request_type, endpoint, data);
        if (!sign.has_value()) {
            return std::nullopt;
        }
        headers["CB-ACCESS-KEY"] = authenticator_.key();
        headers["CB-ACCESS-PASSPHRASE"] = authenticator_.passphrase();
        headers["CB-ACCESS-SIGN"] = sign.value();
        headers["CB-ACCESS-TIMESTAMP"] = ts;
        if (request_type == "GET") {
            return http_client_.get(endpoint, headers);
        } else if (request_type == "POST") {
            return http_client_.post(endpoint, data, headers);
        } else if (request_type == "DELETE") {
            return http_client_.delete_(endpoint, headers);
        }
        return std::nullopt;
    }

    std::optional<nlohmann::json> AuthenticatedApi::get_accounts() {
        return get_account("");
    }

    std::optional<nlohmann::json> AuthenticatedApi::get_account(const std::string &account) {
        std::string endpoint = "/accounts" + (account.empty() ? "" : "/" + account);
        auto result = send_authenticated_request("GET", endpoint, "");
        if (result.has_value())
            return nlohmann::json::parse(result.value());
        return std::nullopt;
    }

    std::optional<nlohmann::json> AuthenticatedApi::get_account_history(const std::string &account) {
        return get_account(account + "/ledger");
    }

    std::optional<nlohmann::json> AuthenticatedApi::place_order(const Order &order) {
        auto order_str = create_order_message(order);
        auto result = send_authenticated_request("POST", orders_endpoint_, order_str);
        if (result.has_value())
            return nlohmann::json::parse(result.value());
        return std::nullopt;
    }

    std::optional<nlohmann::json> AuthenticatedApi::get_account_holds(const std::string &account) {
        return get_account(account + "/holds");
    }

    std::optional<nlohmann::json> AuthenticatedApi::cancel_order(const std::string &order_id) {
        std::string endpoint = orders_endpoint_ + "/" + order_id;
        auto result = send_authenticated_request("DELETE", orders_endpoint_, "");
        if (result.has_value())
            return nlohmann::json::parse(result.value());
        return std::nullopt;
    }

    std::optional<nlohmann::json> AuthenticatedApi::cancel_all(std::optional<std::string> product_id) {
        std::stringstream ss;
        ss << orders_endpoint_;
        if (product_id.has_value()) {
            ss << "?product_id=";
            ss << product_id.value();
        }
        auto result = send_authenticated_request("DELETE", ss.str(), "");
        if (result.has_value())
            return nlohmann::json::parse(result.value());
        return std::nullopt;
    }

    std::optional<nlohmann::json>
    AuthenticatedApi::get_order(const std::optional<std::string> &status) {
        std::stringstream ss;
        ss << orders_endpoint_;
        if (status.has_value()) {
            ss << "?status=";
            ss << status.value();
        }
        auto result = send_authenticated_request("GET", ss.str(), "");
        if (result.has_value())
            return nlohmann::json::parse(result.value());
        return std::nullopt;
    }

    std::optional<nlohmann::json> AuthenticatedApi::get_order(const std::string &product_id,
                                                              const std::optional<std::string> &status) {
        std::stringstream ss;
        ss << orders_endpoint_;
        ss << "?product_id=";
        ss << product_id;
        if (status.has_value()) {
            ss << "&status=";
            ss << status.value();
        }
        auto result = send_authenticated_request("GET", ss.str(), "");
        if (result.has_value())
            return nlohmann::json::parse(result.value());
        return std::nullopt;
    }

    std::optional<nlohmann::json> AuthenticatedApi::get_fills_for_order(const std::string &order_id) {
        std::string endpoint = "/Fills?order_id=" + order_id;
        auto result = send_authenticated_request("GET", endpoint, "");
        if (result.has_value())
            return nlohmann::json::parse(result.value());
        return std::nullopt;
    }

    std::optional<nlohmann::json> AuthenticatedApi::get_fills_for_product(const std::string &product_id) {
        std::string endpoint = "/Fills?product_id=" + product_id;
        auto result = send_authenticated_request("GET", endpoint, "");
        if (result.has_value())
            return nlohmann::json::parse(result.value());
        return std::nullopt;
    }

    PublicApi::PublicApi(const krypto::Config &config, const std::string &environment) :
            http_client_{
                    config.at<std::string>("/exchanges/coinbase/rest/base_url/" + environment)},
            inst_endpoint_{
                    "/" + config.at<std::string>(
                            "/exchanges/coinbase/rest/channels/instruments")} {
    }

    std::optional<nlohmann::json> PublicApi::get_time() {
        auto result = http_client_.get("/time", {});
        if (result.has_value())
            return nlohmann::json::parse(result.value());
        return std::nullopt;
    }

    std::optional<nlohmann::json> PublicApi::get_products() {
        auto result = http_client_.get(inst_endpoint_, {});
        if (result.has_value())
            return nlohmann::json::parse(result.value());
        return std::nullopt;
    }
}