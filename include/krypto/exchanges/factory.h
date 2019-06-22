#pragma once

#include <iostream>
#include <memory>
#include <unordered_map>

#include <krypto/logger.h>

namespace krypto::common {

    template <typename Base, typename... Args>
    class Factory {
    public:
        template <typename T>
        static std::unique_ptr<Base> make(Args... args) {
            static_assert(std::is_base_of<Base, T>());
            return std::make_unique<T>(std::forward<Args...>(args...));
        }

        friend Base;
    };

}
