#pragma once
#include <string>
#include <unordered_map>
#include <variant>

namespace void_contingency {
namespace core {

class Config {
public:
    static Config& get_instance();

    // File I/O methods
    void load_from_file(const std::string& filename);
    void save_to_file(const std::string& filename) const;

    // Type-safe value access
    template <typename T>
    void set_value(const std::string& key, const T& value) {
        values_[key] = value;
    }

    template <typename T>
    T get_value(const std::string& key, const T& default_value = T()) const {
        auto it = values_.find(key);
        if (it != values_.end()) {
            if (auto value = std::get_if<T>(&it->second)) {
                return *value;
            }
        }
        return default_value;
    }

private:
    Config() = default;
    ~Config() = default;

    // Store values of different types
    std::unordered_map<std::string, std::variant<int, float, std::string, bool>> values_;
};

}  // namespace core
}  // namespace void_contingency