#pragma once
#include <fstream>
#include <string>

namespace void_contingency {
namespace utils {

// Define log levels for different types of messages
enum class LogLevel {
    DEBUG,    // Detailed information for debugging
    INFO,     // General information
    WARNING,  // Warning messages
    ERROR,    // Error messages
    FATAL     // Fatal errors that may crash the application
};

class Logger {
public:
    // Singleton pattern implementation
    static Logger& get_instance();

    // Logger lifecycle methods
    void initialize(const std::string& log_file);          // Set up logging to file
    void log(LogLevel level, const std::string& message);  // Log a message
    void shutdown();                                       // Clean up logging resources

private:
    Logger() = default;   // Private constructor
    ~Logger() = default;  // Private destructor

    std::ofstream log_file_;       // File stream for logging
    bool is_initialized_ = false;  // Track initialization state
};

}  // namespace utils
}  // namespace void_contingency