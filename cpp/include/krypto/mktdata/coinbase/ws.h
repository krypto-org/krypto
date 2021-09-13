#pragma once

#include <vector>

#include <boost/fiber/all.hpp>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>
#include <nlohmann/json.hpp>

#include <krypto/config.h>
#include <krypto/utils/types.h>
#include <krypto/utils/common.h>

namespace wpp = websocketpp;
namespace ip = boost::asio::ip;

namespace krypto::mktdata::coinbase {
    enum class WsConnectionStatus : uint8_t {
        OPEN, FAILED, CLOSE
    };

    class WsConnection {
    public:
        using ws_client_t = websocketpp::client<websocketpp::config::asio_tls_client>;
        using channel_t = boost::fibers::buffered_channel<nlohmann::json>;

        WsConnection(
                zmq::context_t &context,
                const krypto::Config &config,
                const std::string&,
                channel_t &update_channel);

        void on_open(const websocketpp::connection_hdl& hdl);

        void on_close(const websocketpp::connection_hdl &hdl);

        void on_fail(const websocketpp::connection_hdl &hdl);

        void on_message(const websocketpp::connection_hdl &, const ws_client_t::message_ptr& msg);

        void start();

        void stop();

        void send(std::string &&message);

    private:
        std::string uri_;
        WsConnectionStatus status_;
        channel_t &update_channel_;
        std::vector<krypto::utils::Instrument> instruments_;

        ws_client_t client_;
        websocketpp::connection_hdl hdl_;
        std::mutex connection_lock_;
        bool sandbox_only_;

        std::string generate_subscription();
    };
}