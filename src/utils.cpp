#include "../inc/utils.hpp"

#include <algorithm>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iterator>

#include <ncurses.h>


namespace Utils {
    auto
    get_current_time() -> std::string
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


    auto
    check_line_exist(std::filesystem::path &file, std::string_view line) -> bool
    {
        std::ifstream in_file(file);
        return std::ranges::any_of(
            std::istream_iterator<std::string>(in_file),
            std::istream_iterator<std::string>(),
            [&line](std::string_view file_line) -> bool { return line == file_line; }
        );
    }


    auto
    str_is_digit(std::string_view str) -> bool
    {
        return std::ranges::all_of(
            str,
            []( const char &c ) -> bool { return std::isdigit(c); }
        );
    }

    auto
    run_command(std::string_view cmd, size_t buffer_size) -> std::optional<std::pair<std::string, int32_t>>
    {
        std::vector<char> buffer(buffer_size);
        std::string result;

        std::unique_ptr<FILE, decltype(&pclose)> pipe(
            popen(std::string(cmd).c_str(), "r"), pclose
        );

        if (!pipe) {
            return std::nullopt;
        }

        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }

        return std::pair(result, pclose(pipe.release()));
    }

    auto
    term_has_colors() -> bool {
        auto result = run_command(TERM_COLOR_COMMAND);
        if (result != std::nullopt && result->second == EXIT_SUCCESS) {
            return (std::stoi(result->first) >= MIN_COLOR_AMOUNT);
        }
        return false;
    }
} /* namespace Utils */