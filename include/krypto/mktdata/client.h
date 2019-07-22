#pragma once

#include <boost/fiber/all.hpp>

#include <krypto/network/subscriber.h>
#include <krypto/config.h>
#include <krypto/utils/common.h>
#include <krypto/mktdata/book.h>

namespace krypto::mktdata {

    template <krypto::utils::ExchangeType Ex>
    class Client final {
    public:
        using quote_channel_t = boost::fibers::buffered_channel<Quote>;
        using trade_channel_t = boost::fibers::buffered_channel<Trade>;
    private:
        krypto::network::Subscriber<false> subscriber_;
    public:
        explicit Client(const krypto::Config& config);
        void start();
        void stop();
    };

    template<utils::ExchangeType Ex>
    Client<Ex>::Client(const krypto::Config &config) :
    subscriber_{config.at<std::string>(
            fmt::format("/services/mktdata/{0}/client",
            krypto::utils::ExchangeTypeEnum::enum_to_names.at(Ex)))} {
    }

    template<utils::ExchangeType Ex>
    void Client<Ex>::start() {
        // Send data on to a channel
    }

    template<utils::ExchangeType Ex>
    void Client<Ex>::stop() {
        // Close Channel
    }
}
