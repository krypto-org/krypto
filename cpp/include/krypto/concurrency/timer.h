#pragma once

#include <krypto/logger.h>
#include <boost/asio.hpp>
#include <boost/asio/high_resolution_timer.hpp>
#include <chrono>

namespace krypto::concurrency {
    template<typename duration_t>
    class TimerTask {
    private:
        using timer_t = std::shared_ptr<boost::asio::deadline_timer>;
        std::function<void(void)> callable_;
        boost::posix_time::microseconds repeat_time_;
        boost::asio::io_service io_;
        timer_t timer_;
        std::thread run_thread_;

        void handler(const boost::system::error_code & = {});

    public:
        TimerTask(std::function<void(void)> &&, duration_t repeat_time);

        void start();

        void stop();
    };


    template<typename duration_t>
    TimerTask<duration_t>::TimerTask(std::function<void(void)> &&callable,
                                     duration_t repeat_time) :
            callable_(std::move(callable)),
            repeat_time_(
                    std::chrono::duration_cast<std::chrono::microseconds>(
                            repeat_time).count()) {
        timer_ = std::make_shared<timer_t::element_type>(io_);
    }


    template<typename duration_t>
    void TimerTask<duration_t>::handler(const boost::system::error_code &) {
        callable_();
        timer_->expires_from_now(repeat_time_);
        timer_->async_wait([&](auto &&e) {
            handler(e);
        });
    }

    template<typename duration_t>
    void TimerTask<duration_t>::start() {
        run_thread_ = std::thread{[=] {
            handler();
            io_.run();
        }};
    }

    template<typename duration_t>
    void TimerTask<duration_t>::stop() {
        io_.stop();
        run_thread_.join();
    }
}