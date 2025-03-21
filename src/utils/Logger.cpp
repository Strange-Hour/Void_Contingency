#include "utils/Logger.hpp"
#include <ctime>
#include <iostream>

namespace void_contingency {
namespace utils {

Logger& Logger::get_instance() {
    static Logger instance;
    return instance;
}

void Logger::initialize(const std::string& log_file) {
    if (!is_initialized_) {
        log_file_.open(log_file, std::ios::app);
        is_initialized_ = true;
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    if (!is_initialized_)
        return;

    std::time_t now = std::time(nullptr);
    std::string time_str = std::ctime(&now);
    time_str.pop_back();  // Remove newline

    std::string level_str;
    switch (level) {
        case LogLevel::DEBUG:
            level_str = "DEBUG";
            break;
        case LogLevel::INFO:
            level_str = "INFO";
            break;
        case LogLevel::WARNING:
            level_str = "WARNING";
            break;
        case LogLevel::ERROR:
            level_str = "ERROR";
            break;
        case LogLevel::FATAL:
            level_str = "FATAL";
            break;
    }

    log_file_ << "[" << time_str << "] [" << level_str << "] " << message << std::endl;
}

void Logger::shutdown() {
    if (is_initialized_) {
        log_file_.close();
        is_initialized_ = false;
    }
}

}  // namespace utils
}  // namespace void_contingency