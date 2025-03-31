#pragma once
#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <filesystem>
#include <vector>
#include <string>


namespace Utils {
    static const int32_t MICROSECOND_UPPER_BOUND = 1000000;
    static const int32_t MICROSECOND_TO_MILISECOND = 1000;

    static const char *const TERM_COLOR_COMMAND = "tput colors 2> /dev/null";
    static const uint8_t MIN_COLOR_AMOUNT = 16;
    static const size_t DEFAULT_BUFFER_SIZE = 256;

    /// Returns a string of time in the format of: MM:SS:MS
    auto get_current_time() -> std::string;

    /// Checks if a line exists inside a file
    /// @param file the file path of the file that will be checked
    /// @param line the line that will be searched in the file
    /// @returns true if it founds a line that match, or false.
    auto check_line_exist(std::filesystem::path &file, std::string_view line) -> bool;

    /// Checks if the whole string is a digit or not
    auto str_is_digit(std::string_view str) -> bool;

    /// Runs a shell command in the user shell
    /// @param cmd the command that will be executed
    /// @param buffer_size the size of the buffer thatll be used
    /// @returns a pair of the command output and the return code, or a nullopt on error
    auto run_command(
        std::string_view cmd, size_t buffer_size = DEFAULT_BUFFER_SIZE
    ) -> std::optional<std::pair<std::string, int32_t>>;

    /// Checks if current terminal used is able to use 16 or more colors
    auto term_has_colors() -> bool;

    auto contains_substr(std::string_view src, std::string_view substr) -> bool;

    auto str_tolower(std::string_view) -> std::string;
} /* namespace Utils */

#endif /* utils.hpp */