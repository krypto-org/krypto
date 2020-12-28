#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <nlohmann/json.hpp>

#include <krypto/exchanges/coinbase/auth.h>
#include <krypto/logger.h>

using namespace krypto::exchanges::coinbase;

TEST_CASE("No Data", "[authenticator]") {
    std::string key = "d7a4666fe9445a53d385f81ba9bef0c1";
    std::string secret = "nuIT+2NB37UkNNwSIeBGNTtG/N8BMKakNAT6YHOE9K83A3P+A3PYjFutjodPCoFoC0X8+0vCf3CzTkH8svGoMQ==";
    std::string passphrase = "coinbase-pro-trading-testing";
    Authenticator authenticator{key, secret, passphrase};
    std::string ts = "1587922987";
    std::string method = "GET";
    std::string path = "/orders";
    auto signature = authenticator.sign(ts, method, path, "");
    REQUIRE(signature.has_value());
    REQUIRE("tllt90Zp/RS1NWRELFywZlLoimiVI0baY63kbxBkuow=" == signature.value());
}

TEST_CASE("Text Data", "[authenticator]") {
    std::string key = "d7a4666fe9445a53d385f81ba9bef0c1";
    std::string secret = "nuIT+2NB37UkNNwSIeBGNTtG/N8BMKakNAT6YHOE9K83A3P+A3PYjFutjodPCoFoC0X8+0vCf3CzTkH8svGoMQ==";
    std::string passphrase = "coinbase-pro-trading-testing";
    Authenticator authenticator{key, secret, passphrase};
    std::string ts = "1587922987";
    std::string method = "GET";
    std::string path = "/orders";
    std::string body = "Hello World";
    auto signature = authenticator.sign(ts, method, path, body);
    REQUIRE(signature.has_value());
    REQUIRE("jIQm3z62FxPVlaQeZqqQA25Ql4AnfOvOZ/jtuPRURiw=" == signature.value());
}

TEST_CASE("Json Data", "[authenticator]") {
    std::string key = "d7a4666fe9445a53d385f81ba9bef0c1";
    std::string secret = "nuIT+2NB37UkNNwSIeBGNTtG/N8BMKakNAT6YHOE9K83A3P+A3PYjFutjodPCoFoC0X8+0vCf3CzTkH8svGoMQ==";
    std::string passphrase = "coinbase-pro-trading-testing";
    Authenticator authenticator{key, secret, passphrase};
    std::string ts = "1587925650";
    std::string method = "POST";
    std::string path = "/orders";
    nlohmann::json order;
    order["size"] = 1.0;
    order["price"] = 1.0;
    order["side"] = "buy";
    order["product_id"] = "BTC-USD";
    order["type"] = "limit";
    std::string body = order.dump();
    KRYP_LOG(info, "Body: {}", body);
    auto signature = authenticator.sign(ts, method, path, body);
    REQUIRE(signature.has_value());
    REQUIRE("yEAtx0icf80ztwSf7QzsUjULXe0EGUw8rVXaFCzjVFg=" == signature.value());
}

