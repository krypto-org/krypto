#include <krypto/mktdata/gateway.h>

namespace krypto::mktdata {
    MktdataGateway::MktdataGateway(
            zmq::context_t &context, const krypto::Config &config) :
            frontend_address_{config.at<std::string>(
                    "/services/mktdata_gateway/frontend/server")},
            backend_address_{config.at<std::string>(
                    "/services/mktdata_gateway/backend/server")},
            frontend_{std::make_unique<zmq::socket_t>(context, ZMQ_XSUB)},
            backend_{std::make_unique<zmq::socket_t>(context, ZMQ_XPUB)},
            running_{false} {
    }

    void MktdataGateway::start() {
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
            zmq::proxy(*frontend_, *backend_);
        } else {
            KRYP_LOG(warn, "Failed to start proxy - binding problems");
        }
    }

    void MktdataGateway::stop() {
        running_ = false;
        frontend_->close();
        backend_->close();
    }

}