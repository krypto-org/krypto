#pragma once

#include <vector>

#include <krypto/instruments/loader.h>
#include <krypto/common/factory.h>
#include <krypto/http.h>

namespace krypto::instruments {
    class Coinbase : public InstrumentLoader {
    private:
        krypto::HttpClient http_client_;
        std::string endpoint_;
    public:
        explicit Coinbase(const Config &config);
        std::vector<krypto::serialization::Instrument> load() override;
    };

    Coinbase::Coinbase(const Config &config) :
            http_client_{config.at<std::string>("/exchanges/coinbase/rest/base_url")},
            endpoint_{config.at<std::string>("/exchanges/coinbase/rest/channels/instruments")} {}

    std::vector<krypto::serialization::Instrument> Coinbase::load() {
        return std::vector<krypto::serialization::Instrument>();
    }
}