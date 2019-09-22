#pragma once

#include <array>
#include <cstddef>
#include <chrono>

#include <krypto/logger.h>

namespace krypto::utils {
    template<size_t size>
    class OrderIdGenerator final {
    private:
        static const constexpr int64_t MULTIPLIER = std::pow(10, size - 6);
        int base_id_;
        static int generate_base_id();
    public:
        OrderIdGenerator();
        std::string new_id();
    };

    template<size_t size>
    OrderIdGenerator<size>::OrderIdGenerator() : base_id_{generate_base_id()} {}

    template<size_t size>
    std::string OrderIdGenerator<size>::new_id() {
        return std::to_string(++base_id_);
    }

    template<size_t size>
    int OrderIdGenerator<size>::generate_base_id() {
        static_assert(size >= 7);
        std::time_t t = std::time(nullptr);
        char buffer[7];
        std::strftime(buffer,
                      sizeof(buffer), "%H%M%S",
                      std::localtime(&t));
        buffer[6] = '\0';
        KRYP_LOG(info, "Initialized order id buffer with {}", buffer);

        return std::stoi(std::string(buffer)) * MULTIPLIER;
    }
}