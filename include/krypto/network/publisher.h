#pragma once

#include "krypto/serialization/serialization_generated.h"

namespace krypto::network {
    template<typename T>
    class Publisher {
    public:
        void publish(T &&value);
    };

    template<typename T>
    void Publisher<T>::publish(T &&value) {

    }
} // namespace krypto::network