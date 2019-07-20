#pragma once

#include <atomic>
#include <string>
#include <queue>
#include <unordered_map>

#include <zmq.hpp>

#include <krypto/config.h>
#include <krypto/logger.h>
#include <krypto/network/helper.h>

namespace krypto::network::rpc {
    template<bool Verbose = false>
    class Broker final {
    private:
        zmq::context_t context_;
        std::string frontend_address_;
        std::string backend_address_;
        std::unique_ptr<zmq::socket_t> frontend_;
        std::unique_ptr<zmq::socket_t> backend_;
        std::atomic_bool running_;
        std::unordered_map<std::string, std::string> workers_;

        bool bind();

    public:
        Broker(const krypto::Config &config);

        ~Broker();

        void start();

        void stop();
    };

    template<bool Verbose>
    Broker<Verbose>::Broker(const krypto::Config &config) :
            context_{1},
            frontend_address_{config.at<std::string>("/services/rpc/broker/frontend/server")},
            backend_address_{config.at<std::string>("/services/rpc/broker/backend/server")},
            running_{false} {
        frontend_ = std::make_unique<zmq::socket_t>(context_, ZMQ_ROUTER);
        backend_ = std::make_unique<zmq::socket_t>(context_, ZMQ_ROUTER);
    }


    template<bool Verbose>
    void Broker<Verbose>::start() {
        running_ = bind();

        while (running_) {

            zmq::pollitem_t items[] = {
                    {*backend_,  0, ZMQ_POLLIN, 0},
                    {*frontend_, 0, ZMQ_POLLIN, 0}
            };

            zmq::poll(&items[0], 2, 0);

            if (items[0].revents && ZMQ_POLLIN) {
                auto address = recv_string(*backend_);
                recv_empty_frame(*backend_);

                auto status = recv_status(*backend_);
                auto service_name = recv_string(*backend_);

                if (status == SocketStatus::READY) {
                    KRYP_LOG(info, "{} :: ready @ address {}", service_name, address);
                    workers_[service_name] = address;
                } else if (status == SocketStatus::DISCONNECT) {
                    KRYP_LOG(info, "{} :: disconnected", service_name);
                    workers_.erase(service_name);
                } else if (status == SocketStatus::REPLY) {
                    auto client_addr = recv_string(*backend_);
                    recv_empty_frame(*backend_);

                    zmq::message_t payload;
                    backend_->recv(&payload);

                    send_string(*frontend_, client_addr, ZMQ_SNDMORE);
                    send_empty_frame(*frontend_, ZMQ_SNDMORE);
                    send_string(*frontend_, service_name, ZMQ_SNDMORE);

                    if constexpr (Verbose) {
                        KRYP_LOG(info, "{} :: received reply payload of size {}", service_name, payload.size());
                    }

                    frontend_->send(payload);
                }
            }

            if (items[1].revents && ZMQ_POLLIN) {

                auto client_addr = recv_string(*frontend_);
                recv_empty_frame(*frontend_);
                auto service = recv_string(*frontend_);

                zmq::message_t request_payload;
                frontend_->recv(&request_payload);

                if (workers_.find(service) != std::end(workers_)) {
                    auto worker_addr = workers_.at(service);
                    send_string(*backend_, worker_addr, ZMQ_SNDMORE);
                    send_empty_frame(*backend_, ZMQ_SNDMORE);
                    send_string(*backend_, client_addr, ZMQ_SNDMORE);
                    send_empty_frame(*backend_, ZMQ_SNDMORE);

                    if constexpr (Verbose) {
                        KRYP_LOG(info, "{} sending request to {}@{} with payload size {}",
                                 client_addr, service, worker_addr, request_payload.size());
                    }

                    backend_->send(request_payload);
                } else {
                    KRYP_LOG(info, "Service not available");
                    send_string(*frontend_, client_addr, ZMQ_SNDMORE);
                    send_empty_frame(*frontend_, ZMQ_SNDMORE);
                    send_string(*frontend_, service);
                }
            }
        }
    }

    template<bool Verbose>
    bool Broker<Verbose>::bind() {
        bool bound = false;

        KRYP_LOG(info, "Binding frontend to {}", frontend_address_);
        try {
            frontend_->bind(frontend_address_);
            bound = true;
        }
        catch (zmq::error_t &e) {
            KRYP_LOG(error, "Binding failed with {}", e.what());
        }

        KRYP_LOG(info, "Binding backend to {}", backend_address_);
        try {
            backend_->bind(backend_address_);
            bound = true;
        } catch (zmq::error_t &e) {
            KRYP_LOG(error, "Binding failed with {}", e.what());
        }

        if (!bound) {
            KRYP_LOG(warn, "Failed starting proxy - binding problems");
        }

        return bound;
    }

    template<bool Verbose>
    void Broker<Verbose>::stop() {
        running_ = false;
    }

    template<bool Verbose>
    Broker<Verbose>::~Broker() {
        stop();
    }


}
