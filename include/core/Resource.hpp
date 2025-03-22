#pragma once
#include <memory>
#include <string>
#include <unordered_map>

namespace void_contingency {
namespace core {

// Base class for all game resources
class Resource {
public:
    virtual ~Resource() = default;
    // Load resource from file
    virtual bool load(const std::string& path) = 0;
    // Unload resource and free memory
    virtual void unload() = 0;
    // Check if resource is currently loaded
    virtual bool is_loaded() const = 0;
};

// Manages all game resources
class ResourceManager {
public:
    static ResourceManager& get_instance();

    // Template method for loading typed resources
    template <typename ResourceType>
    std::shared_ptr<ResourceType> load_resource(const std::string& path) {
        auto it = resources_.find(path);
        if (it != resources_.end()) {
            return std::dynamic_pointer_cast<ResourceType>(it->second);
        }

        auto resource = std::make_shared<ResourceType>();
        if (resource->load(path)) {
            resources_[path] = resource;
            return resource;
        }
        return nullptr;
    }

    // Unload all resources
    void unload_all();

private:
    ResourceManager() = default;
    ~ResourceManager() = default;

    // Map of resource paths to their instances
    std::unordered_map<std::string, std::shared_ptr<Resource>> resources_;
};

}  // namespace core
}  // namespace void_contingency