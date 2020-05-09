#pragma once

#include <unordered_set>

#include <boost/algorithm/string.hpp>

#include <krypto/utils/common.h>
#include <krypto/network/rpc/worker.h>
#include <krypto/config.h>
#include <krypto/instruments/loader.h>
#include <krypto/serialization/serialization_generated.h>
#include <krypto/utils/types.h>

namespace krypto::instruments {

    template<bool Verbose = false>
    class Server final {
    private:
        InstrumentLoader store_;
        std::vector<krypto::utils::Instrument> cache_;
        std::unique_ptr<zmq::socket_t> socket_;
        std::string endpoint_;
        uint32_t polling_millis_;
        std::atomic_bool running_;
        flatbuffers::FlatBufferBuilder fb_builder_;
        std::unordered_set<std::string> active_instruments_;
        std::unordered_set<std::string> sandbox_instruments_;

        bool process(const zmq::message_t &msg, krypto::utils::MsgType msg_type);

        void process_request(const krypto::serialization::InstrumentRequest *request);

        std::vector<std::string> parse_symbols(std::string list);

    public:
        Server(zmq::context_t &context, const krypto::Config &config);

        void start();

        void stop();
    };


    template<bool Verbose>
    Server<Verbose>::Server(zmq::context_t &context, const krypto::Config &config) :
            store_{config}, cache_{},
            socket_{std::make_unique<zmq::socket_t>(context, ZMQ_ROUTER)},
            endpoint_{config.at<std::string>("/services/instruments/server")},
            polling_millis_{config.at<uint32_t>("/services/instruments/polling_millis")},
            running_{false} {
        cache_ = store_.load();
        KRYP_LOG(info, "{}", config.get().dump());
        auto active = config.at<std::string>("/services/instruments/active_symbols");
        auto sandbox_symbols = config.at<std::string>("/services/instruments/sandbox_symbols");
        for (auto &&symbol: parse_symbols(active)) {
            active_instruments_.insert(symbol);
        }
        for (auto &&symbol : parse_symbols(sandbox_symbols)) {
            sandbox_instruments_.insert(symbol);
        }
    }

    template<bool Verbose>
    std::vector<std::string> Server<Verbose>::parse_symbols(std::string list) {
        std::vector<std::string> symbols;
        boost::split(symbols, list, boost::is_any_of(","));
        return symbols;
    }

    template<bool Verbose>
    void Server<Verbose>::stop() {
        running_ = false;
    }

    template<bool Verbose>
    void Server<Verbose>::start() {
        if constexpr (Verbose) {
            KRYP_LOG(info, "Starting instruments server @ {}", endpoint_);
        }

        try {
            socket_->bind(endpoint_.c_str());
        }
        catch (zmq::error_t &e) {
            KRYP_LOG(error, "Binding failed with error {}", e.what());
        }

        running_ = true;

        zmq::pollitem_t items[] = {
                {*socket_, 0, ZMQ_POLLIN, 0}
        };

        while (running_) {
            zmq::poll(&items[0], 1, 0);

            if (items[0].revents && ZMQ_POLLIN) {
                auto address = krypto::network::recv_string(*socket_);
                krypto::network::recv_empty_frame(*socket_);
                auto msg_type = krypto::network::recv_msg_type(*socket_);
                zmq::message_t payload_msg;
                socket_->recv(&payload_msg);
                if constexpr (Verbose) {
                    KRYP_LOG(info, "Received request with payload size {} from {}",
                             payload_msg.size(), address);
                }
                bool send_reply = process(payload_msg, msg_type);
                if (send_reply) {
                    krypto::network::send_string(*socket_, address, ZMQ_SNDMORE);
                    krypto::network::send_empty_frame(*socket_, ZMQ_SNDMORE);
                    zmq::message_t result_msg(fb_builder_.GetSize());
                    std::memcpy(result_msg.data(),
                                fb_builder_.GetBufferPointer(), fb_builder_.GetSize());
                    socket_->send(result_msg);
                    if constexpr (Verbose) {
                        KRYP_LOG(info, "Sent result to {}", address);
                    }
                }
            }
        }
    }

    template<bool Verbose>
    bool Server<Verbose>::process(const zmq::message_t &msg, krypto::utils::MsgType msg_type) {
        if (msg_type == krypto::utils::MsgType::INSTRUMENT_REQUEST) {
            auto request = flatbuffers::GetRoot<krypto::serialization::InstrumentRequest>(
                    msg.data());
            process_request(request);
            return true;
        } else if (msg_type == krypto::utils::MsgType::INSTRUMENT_CACHE_REFRESH_REQUEST) {
            flatbuffers::GetRoot<krypto::serialization::InstrumentRefreshRequest>(
                    msg.data());
            cache_.clear();
            cache_ = store_.load();
            return false;
        }
        return false;
    }

    template<bool Verbose>
    void Server<Verbose>::process_request(const krypto::serialization::InstrumentRequest *request) {
        fb_builder_.Clear();
        std::vector<flatbuffers::Offset<krypto::serialization::Instrument>> instruments;
        if (request->type() == krypto::serialization::RequestType_ALL) {
            for (auto &&inst : cache_) {
                auto inst_offset = krypto::serialization::CreateInstrument(
                        fb_builder_, inst.id, inst.inst_type,
                        fb_builder_.CreateString(inst.symbol),
                        inst.exchange,
                        fb_builder_.CreateString(inst.exchange_symbol),
                        inst.tick_size,
                        inst.min_size,
                        inst.max_size,
                        inst.crypto_base,
                        inst.crypto_quote,
                        active_instruments_.find(inst.symbol) !=
                        std::end(active_instruments_),
                        sandbox_instruments_.find(inst.symbol) !=
                        std::end(sandbox_instruments_));
                instruments.emplace_back(inst_offset);
            }
        }
        auto instruments_offset = fb_builder_.CreateVector(instruments);
        krypto::serialization::InstrumentResponseBuilder builder{fb_builder_};
        builder.add_instruments(instruments_offset);
        auto response = builder.Finish();
        fb_builder_.Finish(response);
    }
}


