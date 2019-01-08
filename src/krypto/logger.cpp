#include <spdlog/sinks/dist_sink.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/ansicolor_sink.h>

#include <krypto/logger.h>

namespace {
    std::once_flag g_loggingOnce;
    krypto::Logging *g_instance;
}

struct krypto::Logging::Impl {
    Impl() {
        restoreDefaultSink();
    }

    ::spdlog::logger &getLogger() {
        return *::spdlog::get(LOGGER_NAME);
    }

    static const char *makeLoc(const char *file, int line, const char *func) {
        static thread_local char buf[128];
        sprintf(buf, "(%s:%d %s)", basename(file), line, func);
        return buf;
    }

    void setLevel(::spdlog::level::level_enum newLevel) {
        getLogger().set_level(newLevel);
    }

    void clearPattern() {
        getLogger().set_pattern("%v");
    }

    void replaceSink(::spdlog::sink_ptr p) {
        ::spdlog::drop(LOGGER_NAME);

        auto l = std::make_shared<::spdlog::logger>(LOGGER_NAME, p);

        register_logger(l);

        // set global logging level to debug
        ::spdlog::set_level(::spdlog::level::debug);
    }

    void restoreDefaultSink() {

        auto a = std::make_shared<::spdlog::sinks::ansicolor_stdout_sink_st>();
        a->set_color(spdlog::level::info, a->reset);
        replaceSink(a);
    }

};

krypto::Logging &krypto::Logging::instance() {
    call_once(g_loggingOnce, []() { g_instance = new Logging(); });
    return *g_instance;
}

krypto::Logging::Logging() {
    pImpl_ = new Impl();
}

krypto::Logging::~Logging() {
    delete pImpl_;
}

::spdlog::logger &
krypto::Logging::getLogger() {
    return pImpl_->getLogger();
}

const char *krypto::Logging::makeLoc(const char *file, int line, const char *func) {
    return Impl::makeLoc(file, line, func);
}

void krypto::Logging::setLevelInfo() {
    pImpl_->setLevel(spdlog::level::info);
}

void krypto::Logging::setLevelWarn() {
    pImpl_->setLevel(spdlog::level::warn);
}

void krypto::Logging::setLevelError() {
    pImpl_->setLevel(spdlog::level::err);
}

bool krypto::Logging::setLevel(std::string_view lvl) {
    int idx = 0;

    while (idx <= spdlog::level::off) {
        if (spdlog::level::level_names[idx] == lvl) {
            break;
        }
        ++idx;
    }

    if (idx <= spdlog::level::off) {
        pImpl_->setLevel(static_cast<spdlog::level::level_enum>(idx));

        return true;
    }

    return false;
}

void krypto::Logging::clearPattern() {
    pImpl_->clearPattern();
}

void krypto::Logging::replaceSink(::spdlog::sink_ptr p) {
    pImpl_->replaceSink(p);
}

void krypto::Logging::restoreDefaultSink() {
    pImpl_->restoreDefaultSink();
}

krypto::LineLogger krypto::LineLogger::log(LineLogger::LEVEL levelx) {
    return LineLogger(static_cast<::spdlog::level::level_enum>(levelx));
}

void krypto::LineLogger::log(LineLogger::LEVEL level, std::string_view msg) {
    Logging::instance().getLogger().log(static_cast<::spdlog::level::level_enum>(level), std::string(msg));
}

krypto::LineLogger::LineLogger(::spdlog::level::level_enum level)
        : level(level),
          logger(Logging::instance().getLogger()) {}

krypto::LineLogger::LineLogger(LineLogger &&rhs)
        : level(rhs.level),
          logger(rhs.logger) {
}

krypto::LineLogger::~LineLogger() {}

