#include <krypto/mktdata/ws_server.h>
#include <krypto/mktdata/convert.h>
#include <websocketpp/frame.hpp>


krypto::mktdata::WebsocketServer::WebsocketServer(
        const krypto::Config& config) :
        WebsocketServer::Subscriber(
                config.at<std::string>("/services/publisher/mktdata/proxy/backend/client")),
        port_{config.at<uint16_t >("/services/publisher/mktdata/ws/port")},
        running_{false} {
    server_.init_asio();
    server_.set_open_handler(bind(&WebsocketServer::on_open, this, ::_1));
    server_.set_close_handler(bind(&WebsocketServer::on_close, this, ::_1));
    server_.set_message_handler(bind(&WebsocketServer::on_message, this, ::_1, ::_2));

    message_queue_.set_capacity(2048);
}

void krypto::mktdata::WebsocketServer::ioc_run() {
    KRYP_LOG(info, "Starting websocket server @ {}", port_);

    server_.listen(port_);
    server_.start_accept();

    try {
        server_.run();
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}

void krypto::mktdata::WebsocketServer::on_open(const connection_hdl &hdl) {
    std::lock_guard<std::mutex> guard(connection_lock_);
    connections_.insert(hdl);
}

void krypto::mktdata::WebsocketServer::on_close(const connection_hdl &hdl) {
    std::lock_guard<std::mutex> guard(connection_lock_);
    connections_.erase(hdl);
}

void krypto::mktdata::WebsocketServer::on_message(
        const connection_hdl &hdl,
        const std::shared_ptr<websocketpp::message_buffer::message<websocketpp::message_buffer::alloc::con_msg_manager>>& msg) {
    std::lock_guard<std::mutex> guard(connection_lock_);
    KRYP_LOG(info, "Ignoring message from client");
}

void krypto::mktdata::WebsocketServer::process_messages() {
    running_ = true;
    while (running_) {
        std::lock_guard<std::mutex> guard(connection_lock_);
        std::string msg;
        message_queue_.pop(msg);

        for (auto&& con : connections_) {
            server_.send(con, msg, websocketpp::frame::opcode::text);
        }
    }
}

void krypto::mktdata::WebsocketServer::process(const krypto::serialization::Quote *quote) {
    std::lock_guard<mutex> guard(connection_lock_);
    if (connections_.empty())
        return;

    nlohmann::json msg;
    msg["kind"] = "quote";
    msg["security_id"] = quote->security_id();
    msg["timestamp_second"] = quote->timestamp() / 1000000000;
    msg["timestamp_nano"] = quote->timestamp() % 1000000000;
    msg["bid"] = krypto::mktdata::extract_price(quote->bid());
    msg["ask"] = krypto::mktdata::extract_price(quote->ask());
    msg["bid_quantity"] = krypto::mktdata::extract_quantity(quote->bid_quantity());
    msg["ask_quantity"] = krypto::mktdata::extract_quantity(quote->ask_quantity());
    msg["last"] = krypto::mktdata::extract_price(quote->last());
    msg["last_quantity"] = krypto::mktdata::extract_quantity(quote->last_quantity());
    message_queue_.push(msg.dump());
}

void krypto::mktdata::WebsocketServer::process(const krypto::serialization::Trade *trade) {
    std::lock_guard<mutex> guard(connection_lock_);
    if (connections_.empty())
        return;

    nlohmann::json msg;
    msg["kind"] = "quote";
    msg["security_id"] = trade->security_id();
    msg["timestamp_second"] = trade->timestamp() / 1000000000;
    msg["timestamp_nano"] = trade->timestamp() % 1000000000;
    msg["price"] = krypto::mktdata::extract_price(trade->price());
    msg["quantity"] = krypto::mktdata::extract_quantity(trade->quantity());
    msg["side"] = krypto::serialization::EnumNameSide(trade->side());
    msg["trade_id"] = trade->trade_id()->str();
    message_queue_.push(msg.dump());
}

void krypto::mktdata::WebsocketServer::done() {
    running_ = false;
}
