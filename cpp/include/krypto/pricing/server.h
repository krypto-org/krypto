#pragma once

#include <limits>
#include <unordered_map>

#include <krypto/mktdata/convert.h>
#include <krypto/network/helper.h>
#include <krypto/utils/types.h>
#include <krypto/pricing/pub.h>
#include <krypto/pricing/ewma.h>
#include <krypto/logger.h>

namespace krypto::pricing {
    class PricingServer final {
    private:
        std::unique_ptr<zmq::socket_t> mktdata_subscriber_;
        Publisher publisher_;
        std::string mktdata_gateway_endpoint_;
        EWMACalculator calculator_;
        std::atomic_bool running_;
        std::unordered_map<int64_t, krypto::utils::TheoreticalSnapshot> theo_snapshots_;

        static double_t calculate_time_delta_seconds(ino64_t previous_time, ino64_t current_time);

    public:
        PricingServer(zmq::context_t &context, const krypto::Config &config);

        void process(const krypto::serialization::Quote *quote);

        void process(const krypto::serialization::Trade *trade);

        void start();

        void stop();
    };
}
