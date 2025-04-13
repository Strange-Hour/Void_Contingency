# Implementing Ship Movement System

## Navigation

- Previous: [Component System Tutorial](./component_system.md)
- Next: [Ship Visualization Tutorial](./visualization.md)

## What We're Building

We're implementing a sophisticated movement system that will control how ships move through space. This system provides realistic physics-based movement with different movement modes and smooth transitions.

## Game Development Concepts Covered

1. **Physics Simulation**: Basic implementation of physics for space movement
2. **Vector Mathematics**: Using vectors for movement calculations
3. **Delta Time**: Smooth movement independent of frame rate
4. **State Machines**: Different movement modes with distinct behaviors
5. **Interpolation**: Smooth transitions between states
6. **Velocity & Acceleration**: Physics concepts in game development
7. **Input Handling**: Responding to player input for movement

## Why This Implementation?

1. **Flexibility**: Multiple movement modes for different gameplay scenarios
2. **Realism**: Physics-based movement for authentic space flight feel
3. **Smoothness**: Proper handling of acceleration and deceleration
4. **Configurability**: Easy to tune movement parameters
5. **Performance**: Efficient calculations for smooth gameplay

## Implementation Steps

### 1. Create Movement Component

Create `include/game/ship/components/MovementComponent.hpp`:

```cpp
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
```

### 2. Implement Movement Component

Create `src/game/ship/components/MovementComponent.cpp`:

```cpp
#include "game/ship/components/MovementComponent.hpp"
#include <cmath>

namespace void_contingency {
namespace game {

MovementComponent::MovementComponent()
    : ShipComponent(ComponentType::Movement)
{
}

void MovementComponent::initialize() {
    // Initialize movement systems
}

void MovementComponent::shutdown() {
    // Clean up movement systems
}

void MovementComponent::update(float deltaTime) {
    if (!ship_) return;

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
    const float maxAngularSpeed = 360.0f; // degrees per second
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

} // namespace game
} // namespace void_contingency
```

### 3. Update Ship Class

Add a method to update the rotation in the Ship class. Edit `include/game/ship/Ship.hpp`:

```cpp
// Basic properties
const std::string& getName() const { return name_; }
float getHealth() const { return health_; }
float getMaxHealth() const { return maxHealth_; }
const core::Transform& getTransform() const { return transform_; }

// Rotation control
void setRotation(float rotation) { transform_.rotation = rotation; }

// Movement
void setVelocity(const Vector2f& velocity) { velocity_ = velocity; }
const Vector2f& getVelocity() const { return velocity_; }
```

This design ensures:

1. The `MovementComponent` controls rotation by using `ship_->setRotation(rotation_)`
2. Position updates are handled by the Ship class's own `update()` method (which adds velocity \* deltaTime to position)
3. The component system respects proper encapsulation and const-correctness

## Key Concepts Explained

### 1. Movement System Design

The movement system is divided into three modes:

1. **Thruster Mode**: Realistic physics-based movement with acceleration and deceleration
2. **Impulse Mode**: Instant velocity changes with gradual deceleration
3. **Hybrid Mode**: A blend of both modes for flexible gameplay

The system is designed with a clear separation of responsibilities:

- The `MovementComponent` handles velocity calculations and rotation
- The `Ship` class handles updating position based on velocity in its own `update()` method
- The transform is encapsulated in the `Ship` class with appropriate getters and setters

### Physics-Based Movement

- Uses realistic physics formulas for movement
- Handles velocity, acceleration, and momentum
- Simulates space-like movement (no friction)
- Supports different movement modes

### Movement Modes

1. **Thruster Mode**:

   - Continuous thrust-based movement
   - Gradual acceleration and deceleration
   - More realistic space movement

2. **Impulse Mode**:

   - Instant velocity changes
   - Quick directional changes
   - Arcade-style movement

3. **Hybrid Mode**:
   - Combines thruster and impulse
   - Balanced between realism and control
   - Customizable blend of both modes

### Vector Mathematics

- 2D vector operations for movement
- Velocity and acceleration calculations
- Direction and magnitude handling
- Angular velocity for rotation

### Movement Parameters

1. **Speed**:

   - Maximum speed limits
   - Current velocity tracking
   - Speed ramping

2. **Acceleration**:

   - Main thrust acceleration
   - Rotational acceleration
   - Deceleration rates

3. **Control**:
   - Input response curves
   - Movement dampening
   - Rotation control

## Common Pitfalls

1. **Frame Rate Independence**:

   - Always use delta time for movement
   - Don't tie physics to frame rate
   - Handle variable time steps

2. **Floating Point Precision**:

   - Watch for accumulating errors
   - Use appropriate precision levels
   - Implement position correction

3. **Movement Transitions**:

   - Smooth mode switching
   - Handle edge cases
   - Prevent sudden velocity changes

4. **Performance**:
   - Optimize vector calculations
   - Use efficient math operations
   - Cache frequently used values

## Best Practices

1. **Physics Calculations**:

   - Use consistent units
   - Implement proper integration
   - Handle edge cases

2. **Input Handling**:

   - Smooth input processing
   - Input buffering when needed
   - Clear input response

3. **Code Organization**:

   - Separate movement logic
   - Clear parameter management
   - Good documentation

4. **Testing**:
   - Unit tests for movement
   - Edge case testing
   - Performance testing

## Testing

Create test file `tests/unit/game/ship/components/MovementComponentTest.cpp`:

```cpp
TEST(MovementComponentTest, BasicMovement) {
    Ship ship("Test Ship");
    auto movement = std::make_unique<MovementComponent>();

    // Test thruster mode
    movement->setMovementMode(MovementMode::Thruster);
    movement->setThrust(Vector2f(1.0f, 0.0f));
    movement->update(1.0f);

    EXPECT_GT(ship.getVelocity().x, 0.0f);
    EXPECT_NEAR(ship.getVelocity().y, 0.0f, 0.001f);
}

TEST(MovementComponentTest, SpeedLimit) {
    Ship ship("Test Ship");
    auto movement = std::make_unique<MovementComponent>();

    movement->setMaxSpeed(100.0f);
    movement->setThrust(Vector2f(1000.0f, 0.0f));
    movement->update(5.0f);

    EXPECT_LE(ship.getVelocity().length(), 100.0f);
}
```

## Next Steps

1. Add advanced movement features:

   - Inertial dampeners
   - Gravity effects
   - Collision response

2. Implement movement prediction
3. Add movement trails and effects
4. Create movement replay system

## Additional Resources

- [Component System Tutorial](./component_system.md)
- [Ship Visualization Tutorial](./visualization.md)
- [Collision System Tutorial](./collision_system.md)
- [Physics Programming Guide](../guides/physics_programming.md)
