#include "../inc/utils.hpp"

#include <chrono>
#include <ctime>


namespace Utils {
    auto
    Get_Current_Time() -> std::string
    {
        auto now = std::chrono::system_clock::now();
        auto c_now = std::chrono::system_clock::to_time_t(now);

        std::tm tm_info{};
        localtime_r(&c_now, &tm_info);

        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()
        ) % MICROSECOND_UPPER_BOUND;

        return std::format(
            "{:02}:{:02}:{:03}",
            tm_info.tm_min,
            tm_info.tm_sec,
            (ms.count() / MICROSECOND_TO_MILISECOND)
        );
    }
} /* namespace Utils */