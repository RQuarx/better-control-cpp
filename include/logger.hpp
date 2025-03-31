#pragma once
#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <unordered_map>
#include <fstream>
#include <format>
#include <print>

#include "arg_parse.hpp"
#include "utils.hpp"


enum LogLevel : uint8_t {
    Error = 3,
    Warn = 2,
    Info = 1,
    Debug = 0,
};

class
Logger
{
public:
    explicit Logger(ArgParser *arg_parser);
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
        std::string message = std::vformat(fmt, std::make_format_args(args...));
        std::string_view label = (
            use_color
            ? labels.at(log_level).first
            : labels.at(log_level).second
        );

        if (log_file.is_open()) log_to_file(log_level, message);

        if (log_level >= log_threshold) {
            std::println("{} {} {}", Utils::get_current_time(), label, message);
            std::fflush(stdout);
        }
    }

    [[nodiscard]]
    auto get_previous_log() -> std::pair<LogLevel, std::string_view>;

private:
    std::pair<LogLevel, std::string_view> prev_log;
    uint8_t log_threshold = 2;
    std::ofstream log_file;
    bool should_log;
    bool use_color;

    std::unordered_map<LogLevel, std::pair<const char *, const char *>> labels = {
        { Warn, { "\e[1;37m[\e[1;33mWARNING\e[1;37m]:\e[0;0;0m", "[WARNING]:" } },
        { Error, { "\e[1;37m[\e[1;31mERROR\e[1;37m]:\e[0;0;0m ", "[ERROR]: " } },
        { Debug, { "\e[1;37m[\e[1;36mDEBUG\e[1;37m]:\e[0;0;0m ", "[DEBUG]: " } },
        { Info, { "\e[1;37m[\e[1;32mINFO\e[1;37m]:\e[0;0;0m  ", "[INFO]:  " } },
    };

    void
    log_to_file(LogLevel log_level, std::string_view msg);
};

#endif /* logger.hpp */