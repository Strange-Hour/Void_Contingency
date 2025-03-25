#include "game/ship/Ship.hpp"
#include "core/Component.hpp"
#include <algorithm>

namespace void_contingency {
namespace game {

Ship::Ship(const std::string& name)
    : name_(name)
    , health_(100.0f)
    , maxHealth_(100.0f)
    , velocity_(0.0f, 0.0f)
{
}

void Ship::addComponent(std::unique_ptr<Component> component) {
    components_.push_back(std::move(component));
}

template<typename T>
T* Ship::getComponent() const {
    for (const auto& component : components_) {
        if (auto* typed = dynamic_cast<T*>(component.get())) {
            return typed;
        }
    }
    return nullptr;
}

void Ship::update(float deltaTime) {
    // Update position based on velocity
    transform_.position += velocity_ * deltaTime;

    // Update all components
    for (auto& component : components_) {
        component->update(deltaTime);
    }
}

void Ship::damage(float amount) {
    health_ = std::max(0.0f, health_ - amount);
}

void Ship::heal(float amount) {
    health_ = std::min(maxHealth_, health_ + amount);
}

} // namespace game
} // namespace void_contingency