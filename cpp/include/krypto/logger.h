#pragma once

#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>


namespace krypto {
    class Logging {
        Logging();

    public:
        static Logging &instance();

        ~Logging();

        static const char *makeLoc(const char *file, int line, const char *func);

        ::spdlog::logger &getLogger();

        void setLevelInfo();

        void setLevelWarn();

        void setLevelError();

        bool setLevel(std::string_view which);

        void clearPattern();

        void replaceSink(::spdlog::sink_ptr p);

        void restoreDefaultSink();

        [[nodiscard]] auto pushLevel(std::string_view which, bool enable = true);

    private:
        static constexpr char LOGGER_NAME[7] = "krypto";
        struct Impl;
        Impl *pImpl_ = nullptr;
    };


    // can't use the one from Generic.h due to mutual header dependency
    inline auto Logging::pushLevel(std::string_view which, bool enable /* =true */) {
        class DO_NOTHING_TAG {
        };
        class OnScopeExit {
            spdlog::level::level_enum level_;
            bool call_;
        public:
            explicit OnScopeExit(DO_NOTHING_TAG)
                    : call_(false) {}

            OnScopeExit(std::string_view level)
                    : level_(Logging::instance().getLogger().level()), call_(true) {
                Logging::instance().setLevel(level);
            }

            ~OnScopeExit() {
                if (call_)
                    Logging::instance().getLogger().set_level(level_);
            }

            OnScopeExit(OnScopeExit &&o)
                    : level_(o.level_), call_(true) {
                o.call_ = false;
            }

            OnScopeExit(const OnScopeExit &) = delete;

            OnScopeExit *operator=(const OnScopeExit) = delete;
        };

        return enable ? OnScopeExit(which) : OnScopeExit(DO_NOTHING_TAG());
    }

    class LineLogger {
        ::spdlog::level::level_enum level;
        ::spdlog::logger &logger;
    public:
        explicit LineLogger(::spdlog::level::level_enum level);

        LineLogger(LineLogger &&rhs) noexcept ;

        ~LineLogger();

        enum LEVEL {
            l_trace = 0,
            l_debug = 1,
            l_info = 2,
            l_warn = 3,
            l_error = 4,    // spell error differently than spdlog's
            l_critical = 5,
            l_off = 6
        };

        static LineLogger log(LEVEL levelx);

        template<typename... Args>
        static void log(LEVEL level, const char *fmt, const Args &... args) {
            Logging::instance().getLogger().log(static_cast<::spdlog::level::level_enum>(level), fmt, args...);
        }


        static void log(LEVEL level, std::string_view msg);
    };
}


#define KRYP_LOC ::krypto::Logging::makeLoc(__FILE__,__LINE__,__FUNCTION__)

#define KRYP_LOG(level_param, ...) ::krypto::LineLogger::log(krypto::LineLogger::l_##level_param, ##__VA_ARGS__)
