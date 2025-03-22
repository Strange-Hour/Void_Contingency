#include <iostream>
#include "core/Resource.hpp"

namespace void_contingency {
namespace core {

// Singleton instance access
ResourceManager& ResourceManager::get_instance() {
    static ResourceManager instance;
    return instance;
}

// Unload all resources and clear the resource cache
void ResourceManager::unload_all() {
    resources_.clear();
}

}  // namespace core
}  // namespace void_contingency