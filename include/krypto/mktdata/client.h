#pragma once

#include <krypto/network/subscriber.h>
#include <krypto/config.h>
#include <krypto/utils/common.h>
#include <krypto/mktdata/book.h>
#include <boost/algorithm/string.hpp>

namespace krypto::mktdata {

    template<krypto::utils::ExchangeType Ex, typename Consumer, bool Verbose = false>
    class Client : public krypto::network::Subscriber<Consumer, Verbose> {
    public:
        explicit Client(const krypto::Config &config);
    };

    template<krypto::utils::ExchangeType Ex, typename Consumer, bool Verbose>
    Client<Ex, Consumer, Verbose>::Client(const krypto::Config &config) :
            Client<Ex, Consumer, Verbose>::template Subscriber<Consumer, Verbose>{
                    config.at<std::string>(
                            fmt::format("/services/publisher/mktdata/{0}/client",
                                        boost::algorithm::to_lower_copy(
                                                krypto::utils::ExchangeTypeEnum::enum_to_names.at(Ex))))} {

    }

}
