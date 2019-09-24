#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <array>
#include <unordered_map>

namespace krypto::utils {

    template <typename T>
    constexpr uint8_t enum_size() {
        return static_cast<uint8_t>(T::SIZE);
    }

    template <typename T>
    std::array<std::string, enum_size<T>()> enum_names();

    template <typename T>
    struct Enum {
        static constexpr const uint8_t N = enum_size<T>();

        static bool init() {
            auto names = enum_names<T>();
            for (uint8_t i = 0; i < names.size(); ++i) {
                auto t = static_cast<T>(i);
                enum_to_names[t] = names[i];
                names_to_enums[names[i]] = t;
                values[i] = t;
            }
            return true;
        }

        static std::unordered_map<T, std::string> enum_to_names;
        static std::unordered_map<std::string, T> names_to_enums;
        static std::array<T, N> values;
    };

    template <typename T>
    std::unordered_map<T, std::string> Enum<T>::enum_to_names;

    template <typename T>
    std::unordered_map<std::string, T> Enum<T>::names_to_enums;

    template <typename T>
    std::array<T, Enum<T>::N> Enum<T>::values;

    template <typename From, typename To>
    constexpr To convert_compatible_enum(const From from) {
        return static_cast<To>(static_cast<uint8_t>(from));
    };
}
