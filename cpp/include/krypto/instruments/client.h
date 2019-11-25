#pragma once

#include <future>

#include <krypto/network/rpc/client.h>
#include <krypto/serialization/serialization_generated.h>
#include <krypto/utils/common.h>
#include <krypto/utils/message_types.h>
#include <krypto/utils/types.h>
#include <krypto/network/helper.h>

namespace {
    const auto convert_inst_type = krypto::utils::convert_compatible_enum<
            krypto::serialization::InstrumentType, krypto::serialization::InstrumentType>;

    const auto convert_currency = krypto::utils::convert_compatible_enum<
            krypto::serialization::Currency, krypto::serialization::Currency>;
}

namespace krypto::instruments {
    using received_variant_t = std::variant<krypto::utils::InstrumentRequest, krypto::utils::InstrumentCacheRefreshRequest>;

    template<bool Verbose = false>
    class InstrumentClient final {
    private:
        std::unique_ptr<zmq::socket_t> socket_;
        std::string server_endpoint_;
        flatbuffers::FlatBufferBuilder fb_builder_;

        void serialize(krypto::serialization::RequestType request_type);

        std::vector<krypto::utils::Instrument> process(
                const krypto::serialization::InstrumentResponse *response);

    public:
        InstrumentClient(zmq::context_t &context, const krypto::Config &config);

        std::future<std::vector<krypto::utils::Instrument>> query(
                krypto::serialization::RequestType request_type =
                krypto::serialization::RequestType::RequestType_ALL,
                int timeout = -1);

        void refresh_cache();
    };

    template<bool Verbose>
    InstrumentClient<Verbose>::InstrumentClient(
            zmq::context_t &context, const krypto::Config &config) :
            socket_{std::make_unique<zmq::socket_t>(context, ZMQ_DEALER)},
            server_endpoint_{
                    config.at<std::string>("/services/instruments/client")} {
        if constexpr  (Verbose) {
            KRYP_LOG(info, "Connecting to server @ {}", server_endpoint_);
        }
        auto identity = "instruments-" + krypto::network::generate_id();
        krypto::network::connect(*socket_, server_endpoint_, identity);
    }

    template<bool Verbose>
    void InstrumentClient<Verbose>::serialize(
            krypto::serialization::RequestType request_type) {
        fb_builder_.Clear();
        krypto::serialization::InstrumentRequestBuilder builder{fb_builder_};
        builder.add_type(request_type);
        auto req = builder.Finish();
        fb_builder_.Finish(req);
    }

    template<bool Verbose>
    std::vector<krypto::utils::Instrument>
    InstrumentClient<Verbose>::process(
            const krypto::serialization::InstrumentResponse *response) {
        std::vector<krypto::utils::Instrument> result;
        if (response) {
            auto insts = response->instruments();
            KRYP_LOG(info, "Received {} instruments", insts->size());
            for (size_t i = 0; i < insts->size(); ++i) {
                auto inst = insts->Get(i);
                krypto::utils::Instrument to_insert{
                        static_cast<int64_t>(inst->id()),
                        convert_inst_type(inst->type()),
                        inst->symbol()->str(),
                        inst->exchange(),
                        inst->exchange_symbol()->str(),
                        inst->tick_size(),
                        inst->min_size(),
                        inst->max_size(),
                        convert_currency(inst->crypto_base()),
                        convert_currency(inst->crypto_quote())
                };
                result.push_back(to_insert);
            }
        }
        return result;
    }

    template<bool Verbose>
    void InstrumentClient<Verbose>::refresh_cache() {
        krypto::network::send_empty_frame(*socket_, ZMQ_SNDMORE);
        krypto::network::send_msg_type(*socket_,
                                       krypto::utils::MsgType::INSTRUMENT_CACHE_REFRESH_REQUEST, ZMQ_SNDMORE);
        fb_builder_.Clear();
        krypto::serialization::InstrumentRefreshRequestBuilder builder{fb_builder_};
        auto req = builder.Finish();
        fb_builder_.Finish(req);
        zmq::message_t request_msg(fb_builder_.GetSize());
        std::memcpy(request_msg.data(),
                    fb_builder_.GetBufferPointer(),
                    fb_builder_.GetSize());
        socket_->send(request_msg);
    }

    template<bool Verbose>
    std::future<std::vector<krypto::utils::Instrument>>
    InstrumentClient<Verbose>::query(const krypto::serialization::RequestType request_type,
                                     int timeout) {
        krypto::network::send_empty_frame(*socket_, ZMQ_SNDMORE);
        krypto::network::send_msg_type(*socket_, krypto::utils::MsgType::INSTRUMENT_REQUEST, ZMQ_SNDMORE);
        serialize(request_type);
        zmq::message_t request_msg(fb_builder_.GetSize());
        std::memcpy(request_msg.data(),
                    fb_builder_.GetBufferPointer(),
                    fb_builder_.GetSize());
        socket_->send(request_msg);
        zmq::pollitem_t items[] = {
                {*socket_, 0, ZMQ_POLLIN, 0},
        };
        zmq::poll(&items[0], 1, timeout);
        std::promise<std::vector<krypto::utils::Instrument>> result;
        if (items[0].revents && ZMQ_POLLIN) {
            krypto::network::recv_empty_frame(*socket_);
            zmq::message_t response_msg;
            socket_->recv(&response_msg);
            auto response = flatbuffers::GetRoot<
                    krypto::serialization::InstrumentResponse>(
                    response_msg.data());
            result.set_value(process(response));
        }
        return result.get_future();
    }
}
