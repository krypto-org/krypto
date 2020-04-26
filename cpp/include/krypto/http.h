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
        uint16_t port_;
    public:
        explicit HttpClient(std::string);

        HttpClient(const HttpClient& rhs) = default;
        HttpClient(HttpClient&& rhs) = default;
        HttpClient& operator=(const HttpClient& rhs) = default;
        HttpClient& operator=(HttpClient&& rhs) = default;

        ~HttpClient();
        std::optional<std::string> get(const std::string&, const std::unordered_map<std::string, std::string>&);
        std::optional<std::string> post(const std::string&, const std::string&,
                const std::unordered_map<std::string, std::string>&);
    };
}