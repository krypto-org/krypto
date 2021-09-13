#pragma once

#include <krypto/instruments/client.h>
#include <krypto/orders/protocol.h>
#include <krypto/mktdata/protocol.h>
#include <krypto/network/subscriber.h>
#include <krypto/network/rpc/worker.h>
#include <krypto/network/publisher.h>
#include <krypto/exchanges/coinbase/api.h>
#include <krypto/utils/types.h>
#include <krypto/utils/date_time.h>
#include <krypto/utils/message_types.h>
#include <krypto/serialization/helper.h>
#include <krypto/mktdata/convert.h>

#include <utility>

namespace krypto::orders::coinbase {
    class OrderServer final {
    private:
        std::unique_ptr<zmq::socket_t> receiver_;
        std::unique_ptr<zmq::socket_t> mktdata_subscriber_;
        std::string order_gateway_endpoint_;
        std::string mktdata_gateway_endpoint_;
        krypto::exchanges::coinbase::AuthenticatedApi api_;
        std::atomic_bool running_;
        bool is_sandbox_;
        std::unordered_map<std::string, krypto::utils::MsgType> msg_type_ref_;
        flatbuffers::FlatBufferBuilder fb_builder_;
        std::unordered_map<int64_t, krypto::utils::Instrument> instruments_;
        std::unordered_map<std::string, int64_t> exchange_symbol_to_id_map_;
        std::unordered_map<std::string, std::string> order_id_cl_order_id_map_;
        std::unordered_map<std::string, std::string> client_identity_map_;

        void serialize(const krypto::utils::OrderUpdate &);

        void send(const krypto::utils::OrderUpdate &order_update, const std::string &client_identity);

        void send_ready(const std::string &identity);

        void send_disconnect(const std::string &identity);

    public:
        OrderServer(zmq::context_t &,
                    krypto::exchanges::coinbase::AuthenticatedApi api,
                    const krypto::Config &, bool is_sandbox);

        void process(const krypto::serialization::OrderRequest *, const std::string &client_identity);

        void process(const krypto::serialization::OrderCancelRequest *, const std::string &client_identity);

        void process(const krypto::serialization::OrderReplaceRequest *, const std::string &client_identity);

        void process(const krypto::serialization::Heartbeat *hb);

        void process(const krypto::serialization::OrderUpdate *order_update);

        void start();

        void stop();
    };
}
