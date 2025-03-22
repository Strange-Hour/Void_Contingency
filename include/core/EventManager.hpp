#pragma once
#include <functional>
#include <unordered_map>
#include <vector>
#include "Event.hpp"

namespace void_contingency {
namespace core {

class EventManager {
public:
    // Singleton pattern implementation
    static EventManager& get_instance();

    // Template method for type-safe event subscription
    template <typename EventType>
    void subscribe(std::function<void(const EventType&)> callback) {
        callbacks_[typeid(EventType)].push_back(
            [callback](const Event& event) { callback(static_cast<const EventType&>(event)); });
    }

    // Emit an event to all registered callbacks
    void emit(const Event& event);
    // Clear all registered callbacks
    void clear();

private:
    EventManager() = default;
    ~EventManager() = default;

    // Map of event types to their callback functions
    std::unordered_map<std::type_index, std::vector<std::function<void(const Event&)>>> callbacks_;
};

}  // namespace core
}  // namespace void_contingency