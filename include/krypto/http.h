#pragma once

#include <string>
#include <boost/optional.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>

namespace krypto {
    class HttpClient final {
    private:
        std::string host_;
        int16_t port_;
    public:
        HttpClient(std::string&&, int16_t);
        ~HttpClient();
        std::optional<std::string> get(std::string&&);
        std::optional<std::string> post(std::string&&, std::string&&);
    };
}