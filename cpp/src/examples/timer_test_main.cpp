#include <krypto/concurrency/timer.h>
#include <krypto/logger.h>

int main() {
    krypto::concurrency::TimerTask<std::chrono::milliseconds> timer_task{[&] {
        KRYP_LOG(info, "Tick");
    }, std::chrono::milliseconds{10}};
    timer_task.start();
    std::this_thread::sleep_for(std::chrono::seconds{2});
    timer_task.stop();
}