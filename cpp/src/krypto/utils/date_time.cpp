#include <krypto/utils/date_time.h>


std::chrono::microseconds krypto::utils::parse_8601(const std::string &str) {
    namespace bpt = boost::posix_time;
    static const bpt::ptime time_t_epoch{boost::gregorian::date{1970, 1, 1}};
    thread_local std::stringstream ss = []() {
        using boost::posix_time::time_input_facet;
        using std::locale;

        std::stringstream result;
        result.imbue(locale{locale::classic(), new time_input_facet{"%Y%m%dT%H:%M:%S%F"}});
        return result;
    }();

    ss.clear();
    ss.str(str);
    bpt::ptime time;
    ss >> time;

    const auto diff = time - time_t_epoch;

    return std::chrono::microseconds{diff.ticks()};
}

int64_t krypto::utils::current_time_in_nanoseconds() {
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
}
