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

    Config::Config(std::string &&file_path) : root_{} {
        std::ifstream file(file_path);
        root_ << file;
        root_ = root_.flatten();

        KRYP_LOG(info, root_.dump(4));
    }

    template<typename T>
    const T Config::at(std::string &&path) const {
        return root_.at(path).get<T>();
    }

    const nlohmann::json &Config::get() const {
        return root_;
    }
}