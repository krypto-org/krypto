#include <krypto/network/rpc/broker.h>
#include <krypto/logger.h>
#include <krypto/network/helper.h>

krypto::network::rpc::Broker::Broker(zmq::context_t & context,
        std::string frontend, std::string backend) : context_{context},
        frontend_address_{std::move(frontend)}, backend_address_{std::move(backend)},
        frontend_{std::make_unique<zmq::socket_t>(context, ZMQ_ROUTER)},
        backend_{std::make_unique<zmq::socket_t>(context, ZMQ_ROUTER)},
        running_{false} {}

void krypto::network::rpc::Broker::start() {

    running_ = bind();

    while (running_) {
        zmq::pollitem_t items[] = {
                //  Always poll for worker activity on backend
                { *backend_, 0, ZMQ_POLLIN, 0 },
                //  Poll front-end only if we have available workers
                { *frontend_, 0, ZMQ_POLLIN, 0 }
        };

//        if (!workers_.empty())
            zmq::poll(&items[0], 2, 1000);
//        else
//            zmq::poll(&items[0], 1, 1000);

        KRYP_LOG(info, "POLLING [0] -- {} {} {}", items[0].revents, items[0].fd, items[0].events);
        KRYP_LOG(info, "POLLING [1] -- {} {} {}", items[1].revents, items[1].fd, items[1].events);

        if (items[0].revents && ZMQ_POLLIN) {
            auto address = recv_string(*backend_);
            recv_empty_frame(*backend_);

            auto status = recv_status(*backend_);
            auto service_name = recv_string(*backend_);

            KRYP_LOG(info, "Received Update from {}", service_name);

            if (status == SocketStatus::READY) {
                KRYP_LOG(info, "{} : READY", service_name);
                workers_[service_name] = address;
            } else if (status == SocketStatus::DISCONNECT) {
                KRYP_LOG(info, "{} : DISCONNECT", service_name);
                workers_.erase(service_name);
            } else if (status == SocketStatus::REPLY) {
                KRYP_LOG(info, "{} : REPLY", service_name);
                auto client_addr = recv_string(*backend_);
                recv_empty_frame(*backend_);

                zmq::message_t payload;
                backend_->recv(&payload);
                send_string(*frontend_,client_addr, ZMQ_SNDMORE);
                send_empty_frame(*frontend_, ZMQ_SNDMORE);
                send_string(*frontend_, service_name, ZMQ_SNDMORE);
                frontend_->send(payload);
            }
        }

        if (items[1].revents && ZMQ_POLLIN) {
            KRYP_LOG(info, "Received Client Message");

            auto client_addr = recv_string(*frontend_);

            KRYP_LOG(info, "Address: {}", client_addr);

            recv_empty_frame(*frontend_);

            auto service = recv_string(*frontend_);

            KRYP_LOG(info, "Service: {}", service);

            zmq::message_t request_payload;
            frontend_->recv(&request_payload);

            KRYP_LOG(info, "Payload with size {}", request_payload.size());

            if (workers_.find(service) != std::end(workers_)) {
                auto worker_addr = workers_.at(service);
                send_string(*backend_, worker_addr, ZMQ_SNDMORE);
                send_empty_frame(*backend_, ZMQ_SNDMORE);
                send_string(*backend_, client_addr, ZMQ_SNDMORE);
                send_empty_frame(*backend_, ZMQ_SNDMORE);

                backend_->send(request_payload);
            } else {
                KRYP_LOG(info, "Service Not Available");

                send_string(*frontend_,client_addr, ZMQ_SNDMORE);
                send_empty_frame(*frontend_, ZMQ_SNDMORE);
                send_string(*frontend_, service);
            }
        }
    }
}

bool krypto::network::rpc::Broker::bind() {
    bool bound = false;

    KRYP_LOG(info, "BINDING FRONTEND TO {}", frontend_address_);
    try {
        frontend_->bind(frontend_address_);
        bound = true;
    }
    catch (zmq::error_t& e) {
        KRYP_LOG(error, "Binding Failed With {}", e.what());
    }

    KRYP_LOG(info, "BINDING BACKEND TO {}", backend_address_);
    try {
        backend_->bind(backend_address_);
        bound = true;
    } catch (zmq::error_t& e) {
        KRYP_LOG(error, "Binding Failed With {}", e.what());
    }

    if (!bound) {
        KRYP_LOG(warn, "Failed Starting Proxy - Binding Problems");
    }

    return bound;
}

void krypto::network::rpc::Broker::stop() {
    running_ = false;
}
