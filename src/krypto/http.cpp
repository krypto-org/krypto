#include <krypto/http.h>
#include <krypto/logger.h>
#include <krypto/root_certificates.h>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

using tcp = boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;
namespace http = boost::beast::http;

krypto::HttpClient::HttpClient(std::string host) : host_{std::move(host)}, port_{443} {
}

std::optional<std::string> krypto::HttpClient::get(const std::string &endpoint) {

    KRYP_LOG(info, "Requesting {}:{}/{}", host_, port_, endpoint);

    boost::asio::io_context ioc;
    ssl::context ctx{ssl::context::sslv23_client};

    load_root_certificates(ctx);

    tcp::resolver resolver{ioc};
    ssl::stream<tcp::socket> stream{ioc, ctx};

    if (!SSL_set_tlsext_host_name(stream.native_handle(), host_.c_str())) {
        boost::system::error_code ec{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
        throw boost::system::system_error{ec};
    }

    auto const results = resolver.resolve(host_, std::to_string(port_));

    boost::asio::connect(stream.next_layer(), results.begin(), results.end());

    stream.handshake(ssl::stream_base::client);

    http::request<http::string_body> req{http::verb::get, endpoint, 11};
    req.set(http::field::host, host_);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    http::write(stream, req);
    boost::beast::flat_buffer buffer;

    http::response<http::dynamic_body> res;

    http::read(stream, buffer, res);

    boost::system::error_code ec;
    stream.shutdown(ec);

    if (ec == boost::asio::error::eof) {
        ec.assign(0, ec.category());
    } else if (ec == boost::asio::ssl::error::stream_truncated) {
    } else {
        return std::optional<std::string>{};
    }

    if (res.result() != boost::beast::http::status::ok) {
        KRYP_LOG(warn, "STATUS {}[{}] : {}", res.result(), res.result_int(), boost::beast::buffers_to_string(res.body().data()));
        return std::optional<std::string>{};
    }

    return std::optional<std::string>{boost::beast::buffers_to_string(res.body().data())};
}

std::optional<std::string> krypto::HttpClient::post(const std::string&, const std::string&) {
    return std::optional<std::string>();
}

krypto::HttpClient::~HttpClient() {
}

