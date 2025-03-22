#include "core/Config.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

namespace void_contingency {
namespace core {

// Singleton instance access
Config& Config::get_instance() {
    static Config instance;
    return instance;
}

// Load configuration from file
void Config::load_from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open config file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            // Remove whitespace
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            // Try to convert value to appropriate type
            try {
                if (value == "true" || value == "false") {
                    values_[key] = (value == "true");
                } else if (value.find('.') != std::string::npos) {
                    values_[key] = std::stof(value);
                } else {
                    values_[key] = std::stoi(value);
                }
            } catch (...) {
                values_[key] = value;
            }
        }
    }
}

// Save configuration to file
void Config::save_to_file(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open config file for writing: " << filename << std::endl;
        return;
    }

    for (const auto& [key, value] : values_) {
        file << key << " = ";
        std::visit([&file](const auto& v) { file << v << std::endl; }, value);
    }
}

}  // namespace core
}  // namespace void_contingency