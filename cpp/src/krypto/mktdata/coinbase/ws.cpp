#include <utility>

#include <boost/asio/ssl/context.hpp>

#include <krypto/instruments/client.h>
#include <krypto/mktdata/coinbase/ws.h>
#include <krypto/logger.h>


krypto::mktdata::coinbase::WsConnection::WsConnection(
        zmq::context_t &context,
        const krypto::Config &config,
        const std::string &environment,
        krypto::mktdata::coinbase::WsConnection::channel_t &update_channel) :
        uri_{config.at<std::string>(
                "/exchanges/coinbase/ws/base_url/" + environment)},
        status_{WsConnectionStatus::CLOSE},
        update_channel_{update_channel}, sandbox_only_{environment == "sandbox"} {

    krypto::instruments::InstrumentClient client{context, config};
    auto query = client.query().get();
    instruments_.insert(instruments_.end(), std::begin(query), std::end(query));

    client_.init_asio();
    client_.set_open_handler([this](auto &&connection) { on_open(std::forward<decltype(connection)>(connection)); });
    client_.set_close_handler([this](auto &&connection) { on_close(std::forward<decltype(connection)>(connection)); });
    client_.set_fail_handler([this](auto &&connection) { on_fail(std::forward<decltype(connection)>(connection)); });
    client_.set_message_handler(
            [this](auto &&connection, auto &&message) {
                on_message(std::forward<decltype(connection)>(connection), std::forward<decltype(message)>(message));
            });
    client_.set_tls_init_handler([](const websocketpp::connection_hdl &) {
        return websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv12);
    });

}


void krypto::mktdata::coinbase::WsConnection::on_open(const wpp::connection_hdl &hdl) {
    KRYP_LOG(info, "Opened websocket connection to {}", uri_);
    std::lock_guard<std::mutex> guard(connection_lock_);
    status_ = WsConnectionStatus::OPEN;

    auto subscription = generate_subscription();
    KRYP_LOG(info, "Sending subscription: {}", subscription);
    send(std::move(subscription));
}

void krypto::mktdata::coinbase::WsConnection::on_close(const wpp::connection_hdl &hdl) {
    KRYP_LOG(warn, "Closed websocket connection to {}", uri_);
    std::lock_guard<std::mutex> guard(connection_lock_);
    status_ = WsConnectionStatus::CLOSE;
}

void krypto::mktdata::coinbase::WsConnection::on_fail(const wpp::connection_hdl &hdl) {
    KRYP_LOG(error, "Websocket connection to {} failed", uri_);
    std::lock_guard<std::mutex> guard(connection_lock_);
    status_ = WsConnectionStatus::FAILED;
}

void
krypto::mktdata::coinbase::WsConnection::on_message(const wpp::connection_hdl &, const ws_client_t::message_ptr &msg) {
    if (msg->get_opcode() == wpp::frame::opcode::text) {
        auto payload = nlohmann::json::parse(msg->get_payload());
        update_channel_.push(payload);
    } else {
        KRYP_LOG(warn, "Non-text message received");
    }
}

void krypto::mktdata::coinbase::WsConnection::start() {
    wpp::lib::error_code ec;
    ws_client_t::connection_ptr con = client_.get_connection(uri_, ec);
    if (ec) {
        KRYP_LOG(info, "Websocket connection to {} failed: {}", uri_, ec.message());
        return;
    }
    hdl_ = con->get_handle();
    client_.connect(con);
    client_.run();

    KRYP_LOG(info, "Websocket IO loop stopped");
}

void krypto::mktdata::coinbase::WsConnection::send(std::string &&message) {
    auto json = nlohmann::json::parse(message);
    wpp::lib::error_code ec;
    client_.send(hdl_, json.dump(), wpp::frame::opcode::text, ec);
    if (ec) {
        KRYP_LOG(error, "Send error: {}", ec.message());
    }
}

void krypto::mktdata::coinbase::WsConnection::stop() {
    std::lock_guard<std::mutex> guard(connection_lock_);
    client_.stop();
}

std::string krypto::mktdata::coinbase::WsConnection::generate_subscription() {
    nlohmann::json subscription;
    subscription["type"] = "subscribe";
    subscription["channels"] = {"level2", "heartbeat", "full"};
    std::vector<std::string> symbols;

    for (auto &&inst: instruments_) {
        if (sandbox_only_) {
            if (inst.sandbox_enabled) {
                symbols.emplace_back(inst.exchange_symbol);
            }
        } else if (inst.active) {
            symbols.emplace_back(inst.exchange_symbol);
        }
    }

    subscription["product_ids"] = symbols;
    return subscription.dump();
}
