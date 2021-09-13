#include <krypto/instruments/client.h>

namespace krypto::instruments {
    InstrumentClient::InstrumentClient(
            zmq::context_t &context, const krypto::Config &config) :
            socket_{std::make_unique<zmq::socket_t>(context, ZMQ_DEALER)},
            server_endpoint_{
                    config.at<std::string>("/services/instruments/client")} {
        KRYP_LOG(info, "Connecting to server @ {}", server_endpoint_);
        auto identity = "instruments-" + krypto::network::generate_id();
        krypto::network::connect(*socket_, server_endpoint_, identity);
    }

    void InstrumentClient::serialize(
            krypto::serialization::RequestType request_type) {
        fb_builder_.Clear();
        krypto::serialization::InstrumentRequestBuilder builder{fb_builder_};
        builder.add_type(request_type);
        auto req = builder.Finish();
        fb_builder_.Finish(req);
    }

    std::vector<krypto::utils::Instrument>
    InstrumentClient::process(
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
                        convert_currency(inst->crypto_quote()),
                        inst->active(),
                        inst->sandbox_enabled()
                };
                result.push_back(to_insert);
            }
        }
        return result;
    }

    void InstrumentClient::refresh_cache() {
        krypto::network::send_empty_frame(*socket_, zmq::send_flags::sndmore);
        krypto::network::send_msg_type(*socket_,
                                       krypto::utils::MsgType::INSTRUMENT_CACHE_REFRESH_REQUEST,
                                       zmq::send_flags::sndmore);
        fb_builder_.Clear();
        krypto::serialization::InstrumentRefreshRequestBuilder builder{fb_builder_};
        auto req = builder.Finish();
        fb_builder_.Finish(req);
        zmq::message_t request_msg(fb_builder_.GetSize());
        std::memcpy(request_msg.data(),
                    fb_builder_.GetBufferPointer(),
                    fb_builder_.GetSize());
        socket_->send(request_msg, zmq::send_flags::none);
    }

    std::future<std::vector<krypto::utils::Instrument>>
    InstrumentClient::query(const krypto::serialization::RequestType request_type,
                            int timeout) {
        krypto::network::send_empty_frame(*socket_, zmq::send_flags::sndmore);
        krypto::network::send_msg_type(*socket_, krypto::utils::MsgType::INSTRUMENT_REQUEST,
                                       zmq::send_flags::sndmore);
        serialize(request_type);
        zmq::message_t request_msg(fb_builder_.GetSize());
        std::memcpy(request_msg.data(),
                    fb_builder_.GetBufferPointer(),
                    fb_builder_.GetSize());
        socket_->send(request_msg, zmq::send_flags::none);
        zmq::pollitem_t items[] = {
                {*socket_, 0, ZMQ_POLLIN, 0},
        };
        zmq::poll(&items[0], 1, timeout);
        std::promise<std::vector<krypto::utils::Instrument>> result;
        if (items[0].revents && ZMQ_POLLIN) {
            krypto::network::recv_empty_frame(*socket_);
            zmq::message_t response_msg;
            const auto response_size = socket_->recv(response_msg, zmq::recv_flags::none);
            if (!response_size.has_value()) {
                KRYP_LOG(error, "Response had a size  = 0");
            }
            auto response = flatbuffers::GetRoot<
                    krypto::serialization::InstrumentResponse>(
                    response_msg.data());
            result.set_value(process(response));
        }
        return result.get_future();
    }

}