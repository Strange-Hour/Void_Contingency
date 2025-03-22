#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace void_contingency {
namespace core {

class ResourceManager {
public:
    // Singleton instance access
    static ResourceManager& get_instance() {
        static ResourceManager instance;
        return instance;
    }

    // Prevent copying
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // Load a resource
    template <typename T>
    std::shared_ptr<T> load_resource(const std::string& path) {
        // TODO: Implement resource loading logic
        return nullptr;
    }

    // Unload all resources
    void unload_all() {
        // TODO: Implement resource unloading logic
    }

private:
    // Private constructor for singleton
    ResourceManager() = default;
    ~ResourceManager() = default;

    // Resource storage
    std::unordered_map<std::string, std::shared_ptr<void>> resources_;
};

}  // namespace core
}  // namespace void_contingency