#pragma once

#include <string>
#include <optional>

namespace krypto::exchanges::coinbase {

    class Authenticator {
    private:
        std::string key_;
        std::string secret_;
        std::string passphrase_;
    public:
        Authenticator(std::string key, std::string secret, std::string passphrase);
        std::optional<std::string> sign(const std::string& time_stamp, const std::string& method, const std::string& path, const std::string& body);
        static std::string get_timestamp();
        const std::string& key();
        const std::string& passphrase();
        static Authenticator from_file(const std::string& file_path);
    };

}
