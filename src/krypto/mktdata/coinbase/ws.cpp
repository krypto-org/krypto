#include <utility>

#include <utility>

#include <krypto/mktdata/coinbase/ws.h>
#include <krypto/logger.h>
#include <boost/asio/ssl/context.hpp>

krypto::mktdata::coinbase::WsConnection::WsConnection(
        std::string uri, std::function<void(nlohmann::json)> handler, std::string subscription) : uri_(std::move(
        uri)), status_(WsConnectionStatus::CLOSE), handler_(std::move(handler)), subscription_(
        std::move(subscription)) {
    client_.init_asio();
    client_.set_open_handler(std::bind(&WsConnection::on_open, this, std::placeholders::_1));
    client_.set_close_handler(std::bind(&WsConnection::on_close, this, std::placeholders::_1));
    client_.set_close_handler(std::bind(&WsConnection::on_close, this, std::placeholders::_1));
    client_.set_fail_handler(std::bind(&WsConnection::on_fail, this, std::placeholders::_1));
    client_.set_message_handler(
            std::bind(&WsConnection::on_message, this, std::placeholders::_1, std::placeholders::_2));
    client_.set_tls_init_handler([](websocketpp::connection_hdl) {
        return websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv12);
    });

}

void krypto::mktdata::coinbase::WsConnection::on_open(wpp::connection_hdl hdl) {
    KRYP_LOG(info, "Opened Websocket Connection to {}", uri_);
    std::lock_guard<std::mutex> guard(connection_lock_);
    status_ = WsConnectionStatus::OPEN;

    send(subscription_);
}

void krypto::mktdata::coinbase::WsConnection::on_close(wpp::connection_hdl hdl) {
    KRYP_LOG(info, "Closed Websocket Connection to {}", uri_);
    std::lock_guard<std::mutex> guard(connection_lock_);
    status_ = WsConnectionStatus::CLOSE;
}

void krypto::mktdata::coinbase::WsConnection::on_fail(wpp::connection_hdl hdl) {
    KRYP_LOG(info, "Websocket Connection to {} Failed", uri_);
    std::lock_guard<std::mutex> guard(connection_lock_);
    status_ = WsConnectionStatus::FAILED;
}

void krypto::mktdata::coinbase::WsConnection::on_message(wpp::connection_hdl, ws_client_t::message_ptr msg) {
    if (msg->get_opcode() == wpp::frame::opcode::text) {
        handler_(nlohmann::json::parse(msg->get_payload()));
    } else {
        KRYP_LOG(warn, "NON TEXT MESSGAE RECEIVED");
    }
}

void krypto::mktdata::coinbase::WsConnection::start() {
    wpp::lib::error_code ec;
    ws_client_t::connection_ptr con = client_.get_connection(uri_, ec);
    if (ec) {
        KRYP_LOG(info, "Websocket Connection to {} Failed: {}", uri_, ec.message());
        return;
    }
    hdl_ = con->get_handle();
    client_.connect(con);
    client_.run();
}

void krypto::mktdata::coinbase::WsConnection::send(std::string message) {
    auto json = nlohmann::json::parse(message);
    wpp::lib::error_code ec;
    client_.send(hdl_, json.dump(), wpp::frame::opcode::text, ec);
    if (ec) {
        KRYP_LOG(error, "Send Error: {}", ec.message());
    }
}

void krypto::mktdata::coinbase::WsConnection::stop() {
    std::lock_guard<std::mutex> guard(connection_lock_);
    client_.stop();
}
