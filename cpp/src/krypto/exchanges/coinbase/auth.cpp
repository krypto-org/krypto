#include <krypto/exchanges/coinbase/auth.h>

#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>
#include <cryptopp/cryptlib.h>
#include <cryptopp/hmac.h>
#include <cryptopp/sha.h>
#include <cryptopp/base64.h>
#include <cryptopp/filters.h>

#include <krypto/logger.h>



namespace krypto::exchanges::coinbase {
    using CryptoPP::Exception;
    using CryptoPP::HMAC;
    using CryptoPP::SHA256;
    using CryptoPP::Base64Decoder;
    using CryptoPP::Base64Encoder;
    using CryptoPP::StringSink;
    using CryptoPP::StringSource;
    using CryptoPP::HashFilter;

    Authenticator::Authenticator(std::string key, std::string secret, std::string passphrase) :
            key_(std::move(key)), secret_(std::move(secret)), passphrase_(std::move(passphrase)) {
    }

    std::optional<std::string> Authenticator::sign(
            const std::string &time_stamp,
            const std::string &method,
            const std::string &path,
            const std::string &body) {
        std::string mac, encoded, key;
        std::string plain = time_stamp + method + path + body;
        StringSource(secret_, true,
                     new Base64Decoder(
                             new StringSink(key)));
        try {
            HMAC<SHA256> hmac((unsigned char *) key.c_str(), key.length());
            StringSource(plain, true,
                         new HashFilter(hmac,
                                        new StringSink(mac)));
        }
        catch (const CryptoPP::Exception &e) {
            KRYP_LOG(error, "{}", e.what());
            return std::nullopt;
        }
        StringSource(mac, true,
                     new Base64Encoder(
                             new StringSink(encoded)));
        encoded.erase(44, 1);
        return encoded;
    }

    std::string Authenticator::get_timestamp() {
        time_t t = time(nullptr);
        return std::to_string(t);;
    }

    const std::string &Authenticator::passphrase() {
        return passphrase_;
    }

    const std::string &Authenticator::key() {
        return key_;
    }

    Authenticator Authenticator::from_file(const std::string& file_path) {
        std::ifstream f(file_path);
        nlohmann::json contents = nlohmann::json::parse(f);
        std::cout << contents.dump() << '\n';
        f.close();
        return Authenticator(
                contents.at("key").get<std::string>(),
                        contents.at("secret").get<std::string>(),
                                contents.at("passphrase").get<std::string>());

    }

}
