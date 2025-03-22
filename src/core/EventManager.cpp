#include "core/EventManager.hpp"
#include <iostream>

namespace void_contingency {
namespace core {

// Singleton instance access
EventManager& EventManager::get_instance() {
    static EventManager instance;
    return instance;
}

// Emit an event to all registered callbacks
void EventManager::emit(const Event& event) {
    auto it = callbacks_.find(event.get_type());
    if (it != callbacks_.end()) {
        // Call all registered callbacks for this event type
        for (const auto& callback : it->second) {
            callback(event);
        }
    }
}

// Clear all registered callbacks
void EventManager::clear() {
    callbacks_.clear();
}

}  // namespace core
}  // namespace void_contingency