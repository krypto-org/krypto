#include <krypto/mktdata/ws_server.h>
#include <krypto/mktdata/convert.h>
#include <krypto/utils/date_time.h>
#include <websocketpp/frame.hpp>
#include <krypto/mktdata/protocol.h>


krypto::mktdata::WebsocketServer::WebsocketServer(
        zmq::context_t& context,
        const krypto::Config &config) :
        WebsocketServer::Subscriber(
                context,
                config.at<std::string>("/services/publisher/mktdata/proxy/backend/client")),
        port_{config.at<uint16_t>("/services/publisher/mktdata/ws/port")},
        running_{false},
        next_update_ts_{static_cast<int64_t>(krypto::utils::current_time_in_nanoseconds())},
        throttle_frequency_{config.at<uint64_t >("/services/publisher/mktdata/ws/throttle_frequency_nano")} {
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
        const std::shared_ptr<websocketpp::message_buffer::message<websocketpp::message_buffer::alloc::con_msg_manager>> &msg) {
    std::lock_guard<std::mutex> guard(connection_lock_);
    KRYP_LOG(info, "Ignoring message from client");
}

void krypto::mktdata::WebsocketServer::process_messages() {
    running_ = true;
    while (running_) {
        std::string msg;
        message_queue_.pop(msg);

        std::lock_guard<std::mutex> guard(connection_lock_);
        for (auto &&con : connections_) {
            server_.send(con, msg, websocketpp::frame::opcode::text);
        }
    }
}

void krypto::mktdata::WebsocketServer::process(const krypto::serialization::Quote *quote) {
    quotes_[quote->security_id()] = Quote{
            static_cast<uint64_t >(quote->timestamp()),
            static_cast<uint64_t >(quote->security_id()),
            quote->bid(),
            quote->ask(),
            quote->bid_quantity(),
            quote->ask_quantity(),
            quote->last(),
            quote->last_quantity()
    };

    std::lock_guard<mutex> guard(connection_lock_);
    if (!connections_.empty()) {
        if (quote->timestamp() > next_update_ts_) {
            nlohmann::json to_send;
            to_send["kind"] = "quotes";

            nlohmann::json quotes_array;
            for (auto &&[key, value] : quotes_) {
                nlohmann::json msg;
                msg["security_id"] = key;
                msg["timestamp_second"] = value.timestamp / 1000000000;
                msg["timestamp_nano"] = value.timestamp % 1000000000;
                msg["bid"] = krypto::mktdata::extract_price(value.bid);
                msg["ask"] = krypto::mktdata::extract_price(value.ask);
                msg["bid_quantity"] = krypto::mktdata::extract_quantity(value.bid_qty);
                msg["ask_quantity"] = krypto::mktdata::extract_quantity(value.ask_qty);
                msg["last"] = krypto::mktdata::extract_price(value.last);
                msg["last_quantity"] = krypto::mktdata::extract_quantity(value.last_qty);
                quotes_array.push_back(msg);
            }
            to_send["payload"] = quotes_array;
            message_queue_.push(to_send.dump());
            next_update_ts_ += throttle_frequency_;
        }
    }
}

void krypto::mktdata::WebsocketServer::process(const krypto::serialization::Trade *trade) {
    std::lock_guard<mutex> guard(connection_lock_);
    if (!connections_.empty()) {
        nlohmann::json msg;
        msg["kind"] = "trade";
        msg["security_id"] = trade->security_id();
        msg["timestamp_second"] = trade->timestamp() / 1000000000;
        msg["timestamp_nano"] = trade->timestamp() % 1000000000;
        msg["price"] = krypto::mktdata::extract_price(trade->price());
        msg["quantity"] = krypto::mktdata::extract_quantity(trade->quantity());
        msg["side"] = krypto::serialization::EnumNameSide(trade->side());
        msg["trade_id"] = trade->trade_id()->str();
        message_queue_.push(msg.dump());
    }
}

void krypto::mktdata::WebsocketServer::done() {
    running_ = false;
}

void krypto::mktdata::WebsocketServer::process(const krypto::serialization::Heartbeat *hb) {
    std::lock_guard<mutex> guard(connection_lock_);
    if (!connections_.empty()) {
        nlohmann::json msg;
        msg["kind"] = "heartbeat";
        msg["security_id"] = hb->security_id();
        msg["timestamp_second"] = hb->timestamp() / 1000000000;
        message_queue_.push(msg.dump());
    }
}
