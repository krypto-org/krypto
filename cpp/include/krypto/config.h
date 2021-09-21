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
        explicit Config(const std::string &);

        Config(const Config &) = default;

        Config(Config &&) = default;

        Config &operator=(const Config &other) = default;

        Config &operator=(Config &&other) = default;

        ~Config() = default;

        [[nodiscard]] const nlohmann::json &get() const;

        template<typename T>
        T at(const std::string &) const;
    };

    template<typename T>
    T Config::at(const std::string & path) const {
        return root_.at(path).get<T>();
    }

}