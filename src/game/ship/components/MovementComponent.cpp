#include "game/ship/components/MovementComponent.hpp"
#include <algorithm>  // For std::clamp
#include <cmath>

namespace void_contingency {
namespace game {

MovementComponent::MovementComponent() : ShipComponent(ComponentType::Movement) {}

void MovementComponent::initialize() {
    // Initialize movement systems
}

void MovementComponent::shutdown() {
    // Clean up movement systems
}

void MovementComponent::update(float deltaTime) {
    if (!ship_)
        return;

    switch (mode_) {
        case MovementMode::Thruster:
            updateThrusterMode(deltaTime);
            break;
        case MovementMode::Impulse:
            updateImpulseMode(deltaTime);
            break;
        case MovementMode::Hybrid:
            updateHybridMode(deltaTime);
            break;
    }

    // Update the ship's rotation
    ship_->setRotation(rotation_);
}

void MovementComponent::updateThrusterMode(float deltaTime) {
    applyAcceleration(deltaTime);
    applyDeceleration(deltaTime);
    clampVelocity();
}

void MovementComponent::updateImpulseMode(float deltaTime) {
    // In impulse mode, we only apply deceleration
    applyDeceleration(deltaTime);
    clampVelocity();
}

void MovementComponent::updateHybridMode(float deltaTime) {
    // In hybrid mode, we apply both acceleration and deceleration
    // but with different rates
    applyAcceleration(deltaTime * 0.5f);
    applyDeceleration(deltaTime);
    clampVelocity();
}

void MovementComponent::applyAcceleration(float deltaTime) {
    if (thrust_.lengthSquared() > 0.0f) {
        Vector2f direction = thrust_.normalized();
        Vector2f velocity = ship_->getVelocity();
        velocity += direction * acceleration_ * deltaTime;
        ship_->setVelocity(velocity);
    }
}

void MovementComponent::applyDeceleration(float deltaTime) {
    Vector2f velocity = ship_->getVelocity();
    float speed = velocity.length();

    if (speed > 0.0f) {
        Vector2f direction = velocity / speed;
        float newSpeed = std::max(0.0f, speed - deceleration_ * deltaTime);
        ship_->setVelocity(direction * newSpeed);
    }
}

void MovementComponent::applyAngularAcceleration(float deltaTime) {
    if (std::abs(angularVelocity_) > 0.0f) {
        float sign = (angularVelocity_ > 0.0f) ? 1.0f : -1.0f;
        angularVelocity_ += sign * angularAcceleration_ * deltaTime;
    }
}

void MovementComponent::applyAngularDeceleration(float deltaTime) {
    if (std::abs(angularVelocity_) > 0.0f) {
        float sign = (angularVelocity_ > 0.0f) ? 1.0f : -1.0f;
        float newAngularVelocity = std::abs(angularVelocity_) - angularDeceleration_ * deltaTime;
        angularVelocity_ = (newAngularVelocity > 0.0f) ? sign * newAngularVelocity : 0.0f;
    }
}

void MovementComponent::clampVelocity() {
    Vector2f velocity = ship_->getVelocity();
    float speed = velocity.length();

    if (speed > maxSpeed_) {
        ship_->setVelocity(velocity.normalized() * maxSpeed_);
    }
}

void MovementComponent::clampAngularVelocity() {
    const float maxAngularSpeed = 360.0f;  // degrees per second
    angularVelocity_ = std::clamp(angularVelocity_, -maxAngularSpeed, maxAngularSpeed);
}

float MovementComponent::getCurrentSpeed() const {
    return ship_->getVelocity().length();
}

// Movement control methods
void MovementComponent::setMovementMode(MovementMode mode) {
    mode_ = mode;
}

void MovementComponent::setThrust(const Vector2f& thrust) {
    thrust_ = thrust;
}

void MovementComponent::setRotation(float rotation) {
    rotation_ = rotation;
}

void MovementComponent::setAngularVelocity(float angularVelocity) {
    angularVelocity_ = angularVelocity;
}

void MovementComponent::setMaxSpeed(float speed) {
    maxSpeed_ = speed;
}

void MovementComponent::setAcceleration(float acceleration) {
    acceleration_ = acceleration;
}

void MovementComponent::setDeceleration(float deceleration) {
    deceleration_ = deceleration;
}

void MovementComponent::setAngularAcceleration(float acceleration) {
    angularAcceleration_ = acceleration;
}

void MovementComponent::setAngularDeceleration(float deceleration) {
    angularDeceleration_ = deceleration;
}

}  // namespace game
}  // namespace void_contingency