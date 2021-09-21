#include <krypto/pricing/server.h>

namespace krypto::pricing {

    PricingServer::PricingServer(
            zmq::context_t &context, const krypto::Config &config) :
            mktdata_subscriber_{std::make_unique<zmq::socket_t>(context, ZMQ_SUB)},
            publisher_{context, config.at<std::string>(
                    "/services/pricing/server")},
            mktdata_gateway_endpoint_{config.at<std::string>(
                    "/services/mktdata_gateway/backend/client")},
            calculator_{config.at<double_t>(
                    "/services/pricing/half_life")},
            running_{false} {}


    void PricingServer::process(const krypto::serialization::Quote *quote) {

        double_t bid = krypto::mktdata::extract_price(quote->bid());
        double_t ask = krypto::mktdata::extract_price(quote->ask());

        double_t bid_qty = krypto::mktdata::extract_quantity(quote->bid_quantity());
        double_t ask_qty = krypto::mktdata::extract_quantity(quote->ask_quantity());

        double_t weighted_mid = EWMACalculator::calculate_wma(bid, bid_qty, ask, ask_qty);

        if (theo_snapshots_.find(quote->security_id()) == std::end(theo_snapshots_)) {
            theo_snapshots_[quote->security_id()] = krypto::utils::TheoreticalSnapshot{
                    quote->timestamp(),
                    quote->security_id(),
                    std::numeric_limits<double>::quiet_NaN(),
                    std::numeric_limits<double>::quiet_NaN(),
                    std::numeric_limits<double>::quiet_NaN(),
                    std::numeric_limits<double>::quiet_NaN(),
                    std::numeric_limits<double>::quiet_NaN(),
                    std::numeric_limits<double>::quiet_NaN(),
                    std::numeric_limits<double>::quiet_NaN(),
                    std::numeric_limits<double>::quiet_NaN(),
                    false
            };
        }

        if (!theo_snapshots_.at(quote->security_id()).valid) {
            theo_snapshots_.at(quote->security_id()).price = weighted_mid;
            theo_snapshots_.at(quote->security_id()).adjusted_price = weighted_mid;
            theo_snapshots_.at(quote->security_id()).error = 0;
            const double_t spread = ask - bid;
            theo_snapshots_.at(quote->security_id()).mm_base_spread = spread;
            theo_snapshots_.at(quote->security_id()).mm_base_ask = weighted_mid + 0.5 * spread;
            theo_snapshots_.at(quote->security_id()).mm_base_bid = weighted_mid - 0.5 * spread;
            theo_snapshots_.at(quote->security_id()).bid_liquidity = bid_qty;
            theo_snapshots_.at(quote->security_id()).ask_liquidity = ask_qty;
            theo_snapshots_.at(quote->security_id()).valid = true;
        } else {
            double_t previous_price = theo_snapshots_.at(quote->security_id()).price;
            double_t previous_spread = theo_snapshots_.at(quote->security_id()).mm_base_spread;
            double_t previous_bid_liq = theo_snapshots_.at(quote->security_id()).bid_liquidity;
            double_t previous_ask_liq = theo_snapshots_.at(quote->security_id()).ask_liquidity;

            int64_t previous_time = theo_snapshots_.at(quote->security_id()).timestamp;
            double_t time_delta = calculate_time_delta_seconds(
                    previous_time, quote->timestamp());
            theo_snapshots_.at(quote->security_id()).price =
                    calculator_.calculate_mean(previous_price, weighted_mid, time_delta);
            double_t previous_std = theo_snapshots_.at(quote->security_id()).error;
            theo_snapshots_.at(quote->security_id()).error =
                    calculator_.calculate_std(
                            previous_std,
                            weighted_mid,
                            time_delta,
                            theo_snapshots_.at(quote->security_id()).price);
            const double_t spread = calculator_.calculate_mean(previous_spread, ask - bid, time_delta);

            theo_snapshots_.at(quote->security_id()).mm_base_spread = spread;
            theo_snapshots_.at(quote->security_id()).mm_base_ask = weighted_mid + 0.5 * spread;
            theo_snapshots_.at(quote->security_id()).mm_base_bid = weighted_mid - 0.5 * spread;

            theo_snapshots_.at(quote->security_id()).bid_liquidity =
                    calculator_.calculate_mean(previous_bid_liq, bid_qty, time_delta);
            theo_snapshots_.at(quote->security_id()).ask_liquidity =
                    calculator_.calculate_mean(previous_ask_liq, ask_qty, time_delta);

            // TODO: Trade impact model
            theo_snapshots_.at(quote->security_id()).adjusted_price =
                    theo_snapshots_.at(quote->security_id()).price;

        }

        if (theo_snapshots_.at(quote->security_id()).valid) {
            auto topic = krypto::utils::create_topic(
                    krypto::utils::MsgType::THEO, quote->security_id());
            publisher_.send(topic, theo_snapshots_.at(quote->security_id()));
        }
    }

    void PricingServer::process(const krypto::serialization::Trade *trade) {
        // Impact theo. Ignore for now
    }

    double_t PricingServer::calculate_time_delta_seconds(
            ino64_t previous_time, ino64_t current_time) {
        return (static_cast<double_t >(current_time - previous_time) / 1000000000);
    }

    void PricingServer::start() {
        publisher_.bind();

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

            zmq::poll(&items[0], 1, 10);

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

                switch (msg_type) {
                    case krypto::utils::MsgType::QUOTE: {
                        process(flatbuffers::GetRoot<krypto::serialization::Quote>(payload_msg.data()));
                        break;
                    }
                    case krypto::utils::MsgType::TRADE: {
                        process(flatbuffers::GetRoot<krypto::serialization::Trade>(payload_msg.data()));
                        break;
                    }
                    case krypto::utils::MsgType::HEARTBEAT: {
                    }
                    case krypto::utils::MsgType::ORDER_UPDATE: {
                        // Ignore
                        break;
                    }
                    default: {
                        KRYP_LOG(info, "Ignoring unknown message type = {}", topic);
                    }
                }
            }

        }
    }

    void PricingServer::stop() {
        running_ = false;
        mktdata_subscriber_->disconnect(mktdata_gateway_endpoint_);
    }

}

