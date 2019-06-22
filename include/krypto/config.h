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
        explicit Config(std::string&&);

        template <typename T>
        const T at(std::string&&);
    };

    Config::Config(std::string &&file_path) : root_{} {
        std::ifstream file(file_path);
        root_ << file;
        root_ = root_.flatten();

        KRYP_LOG(info, root_.dump());
    }

    template<typename T>
    const T Config::at(std::string &&path) {
        return root_.at(path).get<T>();
    }
}