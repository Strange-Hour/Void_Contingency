#pragma once

#include "ShipComponent.hpp"
#include "core/Transform.hpp"
#include "core/Vector2f.hpp"

namespace void_contingency {
namespace game {

enum class MovementMode {
    Thruster,    // Continuous thrust-based movement
    Impulse,     // Instant velocity changes
    Hybrid       // Combination of both modes
};

class MovementComponent : public ShipComponent {
public:
    MovementComponent();

    void initialize() override;
    void shutdown() override;
    void update(float deltaTime) override;

    // Movement control
    void setMovementMode(MovementMode mode);
    void setThrust(const Vector2f& thrust);
    void setRotation(float rotation);
    void setAngularVelocity(float angularVelocity);

    // Movement properties
    void setMaxSpeed(float speed);
    void setAcceleration(float acceleration);
    void setDeceleration(float deceleration);
    void setAngularAcceleration(float acceleration);
    void setAngularDeceleration(float deceleration);

    // Getters
    MovementMode getMovementMode() const { return mode_; }
    const Vector2f& getThrust() const { return thrust_; }
    float getRotation() const { return rotation_; }
    float getAngularVelocity() const { return angularVelocity_; }
    float getMaxSpeed() const { return maxSpeed_; }
    float getCurrentSpeed() const;

private:
    void updateThrusterMode(float deltaTime);
    void updateImpulseMode(float deltaTime);
    void updateHybridMode(float deltaTime);

    void applyAcceleration(float deltaTime);
    void applyDeceleration(float deltaTime);
    void applyAngularAcceleration(float deltaTime);
    void applyAngularDeceleration(float deltaTime);

    void clampVelocity();
    void clampAngularVelocity();

    MovementMode mode_{MovementMode::Thruster};
    Vector2f thrust_{0.0f, 0.0f};
    float rotation_{0.0f};
    float angularVelocity_{0.0f};

    float maxSpeed_{100.0f};
    float acceleration_{50.0f};
    float deceleration_{30.0f};
    float angularAcceleration_{180.0f};
    float angularDeceleration_{90.0f};
};

} // namespace game
} // namespace void_contingency