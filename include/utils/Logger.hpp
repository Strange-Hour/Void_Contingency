#pragma once
#include <fstream>
#include <string>

namespace void_contingency {
namespace utils {

enum class LogLevel { DEBUG, INFO, WARNING, ERROR, FATAL };

class Logger {
public:
    static Logger& get_instance();

    void initialize(const std::string& log_file);
    void log(LogLevel level, const std::string& message);
    void shutdown();

private:
    Logger() = default;
    ~Logger() = default;

    std::ofstream log_file_;
    bool is_initialized_ = false;
};

}  // namespace utils
}  // namespace void_contingency