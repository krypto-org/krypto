#include <utility>

#include <krypto/network/proxy.h>
#include <krypto/logger.h>

namespace krypto::network {

    ZmqProxy::ZmqProxy(
            zmq::context_t &context,
            std::string front, std::string back,
            std::initializer_list<std::string> topics) :
            context_(context),
            frontend_address_(std::move(front)),
            backend_address_(std::move(back)),
            topics_{topics},
            frontend_(context, ZMQ_XSUB),
            backend_(context, ZMQ_XPUB) {
    }

    ZmqProxy::~ZmqProxy() {
        frontend_.unbind(frontend_address_);
        backend_.unbind(backend_address_);

        frontend_.close();
        backend_.close();
    }

    void ZmqProxy::start() {
        bool bound = false;

        KRYP_LOG(info, "BINDING FRONTEMD TO {}", frontend_address_);
        try {
            frontend_.bind(frontend_address_);
            bound = true;
        }
        catch (zmq::error_t& e) {
            KRYP_LOG(error, "Binding Failed With {}", e.what());
        }

        KRYP_LOG(info, "BINDING BACKEND TO {}", backend_address_);
        try {
            backend_.bind(backend_address_);
            bound = true;
        } catch (zmq::error_t& e) {
            KRYP_LOG(error, "Binding Failed With {}", e.what());
        }

        if (bound) {
            zmq::proxy(frontend_, backend_, nullptr);
        } else {
            KRYP_LOG(warn, "Failed Starting Proxy - Binding Problems");
        }
    }
}