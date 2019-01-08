#pragma once

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>
#include <nlohmann/json.hpp>

namespace wpp = websocketpp;
namespace ip = boost::asio::ip;

typedef websocketpp::client<websocketpp::config::asio_tls_client> ws_client_t;

namespace krypto::mktdata::coinbase {
    enum class WsConnectionStatus : uint8_t {
        OPEN, FAILED, CLOSE
    };

    class WsConnection {
    public:
        typedef websocketpp::lib::shared_ptr<WsConnection> ptr;
        WsConnection(std::string, std::function<void(nlohmann::json)>, std::string );

        void on_open(websocketpp::connection_hdl hdl);

        void on_close(websocketpp::connection_hdl hdl);

        void on_fail(websocketpp::connection_hdl hdl);

        void on_message(websocketpp::connection_hdl, ws_client_t::message_ptr msg);

        void start();

        void stop();

        void send(std::string message);
    private:
        std::string uri_;
        WsConnectionStatus status_;
        std::function<void(nlohmann::json)> handler_;
        std::string subscription_;

        ws_client_t client_;
        websocketpp::connection_hdl hdl_;
        std::mutex connection_lock_;
    };
}