#include "game/ship/components/EngineComponent.hpp"

namespace void_contingency {
namespace game {

EngineComponent::EngineComponent()
    : ShipComponent(ComponentType::Engine)
{
}

void EngineComponent::initialize() {
    // Initialize engine systems
}

void EngineComponent::shutdown() {
    // Clean up engine systems
}

void EngineComponent::update(float deltaTime) {
    if (!ship_) return;

    // Calculate force based on thrust and efficiency
    float force = thrust_ * efficiency_;

    // Apply force to ship
    Vector2f direction = ship_->getTransform().getForward();
    Vector2f velocity = ship_->getVelocity();
    velocity += direction * force * deltaTime;
    ship_->setVelocity(velocity);
}

void EngineComponent::setThrust(float thrust) {
    thrust_ = std::clamp(thrust, 0.0f, maxThrust_);
}

float EngineComponent::getThrust() const {
    return thrust_;
}

float EngineComponent::getMaxThrust() const {
    return maxThrust_;
}

} // namespace game
} // namespace void_contingency