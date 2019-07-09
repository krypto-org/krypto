#include <utility>

#include <utility>

#pragma once

#include <string>
#include <memory>
#include <deque>
#include <zmq.hpp>

namespace krypto::network::mdp {
    struct Worker;

    class Service final {
        std::string name_;
        std::deque<zmq::message_t> requests_;
        std::vector<std::unique_ptr<Worker>> workers_;

    public:
        Service(std::string name);
    };

    struct Worker final {
        std::string identity;
        std::unique_ptr<Service> service;
        uint64_t expiry;

        Worker(std::string identity, std::unique_ptr<Service> &&service, uint64_t expiry);
    };

    class Broker final {

    };

}