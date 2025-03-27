#include "../include/logger.hpp"
#include <cstring>
#include <thread>


Logger::Logger(ArgParser *arg_parser) : use_color(Utils::term_has_colors())
{
    std::string option;
    if (arg_parser->option_arg(option, { "-l", "--log" })) {
        should_log = true;
        if (Utils::str_is_digit(option)) {
            log_threshold = std::stoi(option);

            /* ? Since a '-' is not counted as a digit,
               ? anything below zero isn't going to be turned into a log threshold
               ? but if log_threshold goes beyond 3, there wont be any logs printedm
               ? even error logs.
            */
            if (log_threshold > 3) {
                log_threshold = 2;
                log(Error, "Invalid log threshold!");
            }
        } else {
            log_file.open(option, std::ios_base::app);
        }
    }

    log(Debug, "Logger instance successfully created");
}


Logger::~Logger()
{
    if (log_file.is_open()) log_file.close();
}


void
Logger::log_to_file(LogLevel log_level, std::string_view msg)
{
    std::jthread([this, log_level, msg]() {
            std::string_view label = labels.at(log_level).second;
            std::println(log_file, "{} {} {}", Utils::get_current_time(), label, msg);
        }
    );
}