#pragma once

#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

#include <krypto/logger.h>

namespace krypto {
    class Config final {
    private:
        nlohmann::json root_;
    public:
        explicit Config(std::string &&);

        Config(const Config &) = default;

        Config(Config &&) = default;

        Config &operator=(const Config &other) = default;

        Config &operator=(Config &&other) = default;

        ~Config() = default;

        const nlohmann::json &get() const;

        template<typename T>
        const T at(std::string &&) const;
    };

    template<typename T>
    const T Config::at(std::string &&path) const {
        return root_.at(path).get<T>();
    }

}