#pragma once

#include <chrono>
#include <ctime>
#include <string>

#include <boost/date_time/posix_time/posix_time.hpp>

namespace krypto::utils {
    std::chrono::microseconds parse_8601(const std::string &str);

    uint64_t current_time_in_nanoseconds();
}
