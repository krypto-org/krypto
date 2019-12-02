#pragma once

#include <vector>

#include <zmq.hpp>

#include <krypto/config.h>

namespace krypto::mktdata {
    // TODO: Convert to last value cache based gateway

    template<bool Verbose = true>
    class MktdataGateway final {
    private:
        std::string frontend_address_;
        std::string backend_address_;
        std::unique_ptr<zmq::socket_t> frontend_;
        std::unique_ptr<zmq::socket_t> backend_;
        std::atomic_bool running_;
    public:
        MktdataGateway(zmq::context_t &context, const krypto::Config &config);

        void start();

        void stop();
    };

    template<bool Verbose>
    MktdataGateway<Verbose>::MktdataGateway(
            zmq::context_t &context, const krypto::Config &config) :
            frontend_address_{config.at<std::string>(
                    "/services/mktdata_gateway/frontend/server")},
            backend_address_{config.at<std::string>(
                    "/services/mktdata_gateway/backend/server")},
            frontend_{std::make_unique<zmq::socket_t>(context, ZMQ_XSUB)},
            backend_{std::make_unique<zmq::socket_t>(context, ZMQ_XPUB)},
            running_{false} {
    }

    template<bool Verbose>
    void MktdataGateway<Verbose>::start() {
        bool bound = false;

        KRYP_LOG(info, "Binding frontend @ {}", frontend_address_);
        try {
            frontend_->bind(frontend_address_);
            bound = true;
        }
        catch (zmq::error_t &e) {
            KRYP_LOG(error, "Binding failed with {}", e.what());
        }

        KRYP_LOG(info, "Binding backend @ {}", backend_address_);
        try {
            backend_->bind(backend_address_);
            bound = true;
        } catch (zmq::error_t &e) {
            KRYP_LOG(error, "Binding Failed With {}", e.what());
        }

        if (bound) {
            running_ = true;
            zmq::proxy(*frontend_, *backend_, nullptr);
        } else {
            KRYP_LOG(warn, "Failed to start proxy - binding problems");
        }
    }

    template<bool Verbose>
    void MktdataGateway<Verbose>::stop() {
        running_ = false;
        frontend_->close();
        backend_->close();
    }
}
