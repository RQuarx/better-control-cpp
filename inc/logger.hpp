#pragma once
#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <unordered_map>
#include <cstdint>
#include <fstream>
#include <string>
#include <format>
#include <print>

#include "utils.hpp"


enum LogLevel : uint8_t {
    Info = 0,
    Error = 1,
    Debug = 2,
};

class
Logger
{
public:
    explicit Logger(std::string log_file_path = "");
    Logger(const Logger &) = delete;
    Logger(Logger &&) = delete;
    auto operator=(const Logger &) -> Logger & = delete;
    auto operator=(Logger &&) -> Logger & = delete;
    ~Logger();

    /// Logs to stdout or stderr based on the log level
    /// @param log_level Info would log to stdout and file, Error would log into stderr and file, and Debug would only log to file
    /// @param fmt format string according to std::format
    /// @param ... additional parameters matching the {} tokens in fmt
    void
    log(LogLevel log_level, std::string_view fmt, auto... args)
    noexcept
    {
        std::string time = Utils::Get_Current_Time();
        std::string message = std::vformat(fmt, std::make_format_args(args...));
        std::string printed_str;
        std::string file_str;

        if (log_level != Debug) {
            printed_str = std::format(
                "{} {} {}",
                time,
                labels.at(log_level).first,
                message
            );
        }
        if (log_file.is_open()) {
            file_str = std::format(
                "{} {} {}",
                time,
                labels.at(log_level).second,
                message
            );
        }

        if (log_level == Debug) {
            if (log_file.is_open()) std::println(log_file, "{}", file_str);
            return;
        }

        if (log_level == Error) {
            if (log_file.is_open()) std::println(log_file, "{}", file_str);
            std::println(stderr, "{}", printed_str);
            return;
        }

        if (log_file.is_open()) std::println(log_file, "{}", file_str);
        std::println("{}", printed_str);
    }

    [[nodiscard]]
    auto get_previous_log() -> std::pair<LogLevel, std::string_view>;

private:
    std::pair<LogLevel, std::string_view> prev_log;
    std::ofstream log_file;
    std::unordered_map<LogLevel, std::pair<const char *, const char *>> labels = {
        { Info, { "\e[1;37m[\e[1;32mINFO\e[1;37m]:\e[0;0;0m", "[INFO]:" } },
        { Error, { "\e[1;37m[\e[1;31mERROR\e[1;37m]:\e[0;0;0m", "[ERROR]:" } },
        { Debug, { "\e[1;37m[\e[1;36mDEBUG\e[1;37m]:\e[0;0;0m", "[DEBUG]:" } }
    };
};

#endif /* logger.hpp */