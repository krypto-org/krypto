#include <krypto/mktdata/redis_publisher.h>
#include <krypto/network/helper.h>

namespace krypto::mktdata {

    RedisPublisher::RedisPublisher(zmq::context_t &context, const krypto::Config &config) :
            mktdata_subscriber_{std::make_unique<zmq::socket_t>(context, ZMQ_SUB)},
            mktdata_gateway_endpoint_{config.at<std::string>(
                    "/services/mktdata_gateway/backend/client")},
            redis_{config.at<std::string>(
                    "/external/redis")},
            running_(false) {
    }

    void RedisPublisher::process(const krypto::serialization::Quote *quote) {

        std::unordered_map<std::string, int64_t> values = {
                {"timestamp", quote->timestamp()},
                {"bid", quote->bid()},
                {"bid_quantity", quote->bid_quantity()},
                {"ask", quote->ask()},
                {"ask_quantity", quote->ask_quantity()},
                {"last", quote->last()},
                {"last_quantity", quote->last_quantity()},
        };
        std::string key = std::to_string(quote->security_id());
        redis_.hmset(key, values.begin(), values.end());
    }

    void RedisPublisher::start() {
        KRYP_LOG(info, "Connecting to market data gateway @ {}",
                 mktdata_gateway_endpoint_);
        mktdata_subscriber_->connect(mktdata_gateway_endpoint_);
        running_ = true;
        const std::string subscription = krypto::utils::MsgTypeNames[static_cast<uint8_t>(
                krypto::utils::MsgType::QUOTE)];
        mktdata_subscriber_->set(zmq::sockopt::subscribe, subscription);

        zmq::pollitem_t items[] = {
                {*mktdata_subscriber_, 0, ZMQ_POLLIN, 0}
        };

        while (running_) {

            zmq::poll(&items[0], 1, -1);

            if (items[0].revents && ZMQ_POLLIN) {

                auto topic = krypto::network::recv_string(*mktdata_subscriber_);
                auto topic_prefix = topic.substr(0, 2);
                auto msg_type = krypto::utils::MessageTypeData::msg_type_reference.at(topic_prefix);

                zmq::message_t payload_msg;
                const auto payload_size = mktdata_subscriber_->recv(payload_msg);

                if (!payload_size.has_value()) {
                    KRYP_LOG(error, "Payload has 0 size");
                    break;
                }

                if (msg_type == krypto::utils::MsgType::QUOTE) { // Else Ignore
                    process(flatbuffers::GetRoot<krypto::serialization::Quote>(payload_msg.data()));
                }
            }
        }
    }

    void RedisPublisher::stop() {
        running_ = false;
        mktdata_subscriber_->disconnect(mktdata_gateway_endpoint_);
    }
}