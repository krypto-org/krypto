#include <utility>

#include <utility>

//
// Created by ksharma on 7/8/19.
//

#include <bits/unique_ptr.h>
#include <krypto/network/majordomo/broker.h>

krypto::network::mdp::Worker::Worker(
        std::string identity, std::unique_ptr<Service> &&service, uint64_t expiry) :
        identity(std::move(identity)), service(std::move(service)), expiry(expiry) {}

krypto::network::mdp::Service::Service(std::string name) : name_(std::move(name)) {}
