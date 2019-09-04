#include <krypto/network/proxy.h>
#include <krypto/logger.h>

namespace krypto::network {

    ZmqProxy::ZmqProxy(const krypto::Config &config, const std::string &service_type) :
            context_{1},
            frontend_address_{config.at<std::string>(
                    fmt::format("/services/publisher/{}/proxy/frontend/server", service_type))},
            backend_address_{config.at<std::string>(
                    fmt::format("/services/publisher/{}/proxy/backend/server", service_type))}, running_{false} {
        frontend_ = std::make_unique<zmq::socket_t>(context_, ZMQ_XSUB);
        backend_ = std::make_unique<zmq::socket_t>(context_, ZMQ_XPUB);
    }

    ZmqProxy::~ZmqProxy() {
        if (running_) {
            stop();
        }
    }

    void ZmqProxy::start() {
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
            zmq::proxy(*frontend_, *backend_, nullptr);
        } else {
            KRYP_LOG(warn, "Failed to start proxy - binding problems");
        }
    }

    void ZmqProxy::stop() {
        running_ = false;
        frontend_->unbind(frontend_address_);
        backend_->unbind(backend_address_);
        frontend_->close();
        backend_->close();
    }
}