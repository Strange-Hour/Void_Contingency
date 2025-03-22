#pragma once
#include <string>
#include <typeindex>

namespace void_contingency {
namespace core {

// Base class for all game events
class Event {
public:
    virtual ~Event() = default;
    // Returns the type of the event for type-safe event handling
    virtual std::type_index get_type() const = 0;
};

// Event triggered when the game starts
class GameStartEvent : public Event {
public:
    std::type_index get_type() const override {
        return typeid(GameStartEvent);
    }
};

// Event triggered when the game ends
class GameEndEvent : public Event {
public:
    std::type_index get_type() const override {
        return typeid(GameEndEvent);
    }
};

// Event triggered when a system encounters a critical error
class SystemFailureEvent : public Event {
public:
    // Constructor takes the name of the failed system
    SystemFailureEvent(const std::string& system_name) : system_name_(system_name) {}

    std::type_index get_type() const override {
        return typeid(SystemFailureEvent);
    }

    // Getter for the failed system's name
    const std::string& get_system_name() const {
        return system_name_;
    }

private:
    std::string system_name_;  // Stores the name of the failed system
};

}  // namespace core
}  // namespace void_contingency