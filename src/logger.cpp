#include "../inc/logger.hpp"
#include <cstring>


Logger::Logger(std::string log_file_path)
{
    if (!log_file_path.empty()) {
        log_file = std::ofstream(log_file_path, std::ios::app);

        if (!log_file.is_open()) {
            log(Error, "Failed to create Logger instance: {}", std::strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    log(Debug, "Logger instance successfully created");
}


Logger::~Logger()
{
    if (log_file.is_open()) log_file.close();
}