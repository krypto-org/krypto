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

    class Server final {
    private:
        InstrumentLoader store_;
        std::vector<krypto::utils::Instrument> cache_;
        std::unique_ptr<zmq::socket_t> socket_;
        std::string endpoint_;
        std::atomic_bool running_;
        flatbuffers::FlatBufferBuilder fb_builder_;
        std::unordered_set<std::string> active_instruments_;
        std::unordered_set<std::string> sandbox_instruments_;

        bool process(const zmq::message_t &msg, krypto::utils::MsgType msg_type);

        void process_request(const krypto::serialization::InstrumentRequest *request);

        static std::vector<std::string> parse_symbols(std::string list);

    public:
        Server(zmq::context_t &context, const krypto::Config &config);

        void start();

        void stop();
    };
}


