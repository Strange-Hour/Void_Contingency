# Implementing Ship Collision System

## Navigation

- Previous: [Ship Visualization Tutorial](./visualization.md)
- Next: [Sector Generation Tutorial](../game_loop/sector_generation.md)

## What We're Building

We're implementing a collision system that handles all physical interactions between ships and other objects in the game. This system ensures realistic collision detection and response while providing ways to handle different types of collisions.

## Game Development Concepts Covered

1. **Collision Detection**: Different methods of detecting object intersections
2. **Physics Simulation**: Basic physics for collision response
3. **Spatial Partitioning**: Efficient broad-phase collision detection
4. **Event Systems**: Handling collision callbacks
5. **Vector Mathematics**: Collision resolution calculations
6. **Collision Shapes**: Different types of collision boundaries
7. **Collision Layers**: Managing what objects can collide
8. **Collision Response**: How objects react to collisions
9. **Collision Avoidance**: AI for preventing collisions

## Why This Implementation?

1. **Efficiency**: Uses spatial partitioning for fast collision detection
2. **Flexibility**: Supports different collision shapes and responses
3. **Robustness**: Handles edge cases and prevents tunneling
4. **Extensibility**: Easy to add new collision types and responses
5. **Debugging**: Provides tools for visualizing and debugging collisions

## Implementation Steps

### 1. Create Collision Component

Create `include/game/ship/components/CollisionComponent.hpp`:

```cpp
#pragma once

#include "ShipComponent.hpp"
#include "physics/CollisionShape.hpp"
#include "physics/CollisionLayer.hpp"
#include <functional>
#include <vector>

namespace void_contingency {
namespace game {

struct CollisionInfo {
    Ship* otherShip{nullptr};
    Vector2f point;
    Vector2f normal;
    float depth;
    float impulse;
};

class CollisionComponent : public ShipComponent {
public:
    CollisionComponent();

    void initialize() override;
    void shutdown() override;
    void update(float deltaTime) override;

    // Collision shape management
    void setCollisionShape(std::unique_ptr<physics::CollisionShape> shape);
    void setCollisionLayer(physics::CollisionLayer layer);
    void setCollisionMask(uint32_t mask);

    // Collision response
    void setElasticity(float elasticity);
    void setFriction(float friction);
    void setRestitution(float restitution);

    // Collision callbacks
    using CollisionCallback = std::function<void(const CollisionInfo&)>;
    void setOnCollision(CollisionCallback callback);
    void setOnCollisionStart(CollisionCallback callback);
    void setOnCollisionEnd(CollisionCallback callback);

    // Collision queries
    bool isColliding() const;
    const std::vector<CollisionInfo>& getCurrentCollisions() const;
    bool testCollision(const Ship* other) const;
    bool testCollision(const Vector2f& point) const;

    // Collision avoidance
    void setAvoidanceEnabled(bool enabled);
    void setAvoidanceRadius(float radius);
    Vector2f calculateAvoidanceForce(const std::vector<Ship*>& nearbyShips) const;

private:
    void updateCollisions();
    void resolveCollision(CollisionInfo& info);
    void applyCollisionResponse(const CollisionInfo& info);
    void updateCollisionCallbacks();

    bool checkCollision(const Ship* other, CollisionInfo& info) const;
    bool checkPointCollision(const Vector2f& point, CollisionInfo& info) const;

    void handleCollisionStart(const CollisionInfo& info);
    void handleCollisionEnd(const CollisionInfo& info);
    void handleCollision(const CollisionInfo& info);

    std::unique_ptr<physics::CollisionShape> collisionShape_;
    physics::CollisionLayer layer_{physics::CollisionLayer::Ship};
    uint32_t collisionMask_{0xFFFFFFFF};

    float elasticity_{0.5f};
    float friction_{0.2f};
    float restitution_{0.3f};

    CollisionCallback onCollision_;
    CollisionCallback onCollisionStart_;
    CollisionCallback onCollisionEnd_;

    std::vector<CollisionInfo> currentCollisions_;
    std::vector<CollisionInfo> previousCollisions_;

    bool avoidanceEnabled_{true};
    float avoidanceRadius_{100.0f};
};

} // namespace game
} // namespace void_contingency
```

### 2. Implement Collision Component

Create `src/game/ship/components/CollisionComponent.cpp`:

```cpp
#include "game/ship/components/CollisionComponent.hpp"
#include "physics/CollisionSystem.hpp"
#include <algorithm>

namespace void_contingency {
namespace game {

CollisionComponent::CollisionComponent()
    : ShipComponent(ComponentType::Collision)
{
}

void CollisionComponent::initialize() {
    // Initialize collision systems
}

void CollisionComponent::shutdown() {
    // Clean up collision systems
}

void CollisionComponent::update(float deltaTime) {
    if (!ship_) return;

    updateCollisions();
    updateCollisionCallbacks();
}

void CollisionComponent::updateCollisions() {
    previousCollisions_ = std::move(currentCollisions_);
    currentCollisions_.clear();

    // Get nearby ships from physics system
    auto nearbyShips = physics::CollisionSystem::getInstance().getNearbyShips(
        ship_->getTransform().position,
        avoidanceRadius_
    );

    // Check collisions with nearby ships
    for (Ship* other : nearbyShips) {
        if (other == ship_) continue;

        auto* otherCollision = other->getComponent<CollisionComponent>();
        if (!otherCollision) continue;

        // Check collision mask
        if (!(layer_ & otherCollision->collisionMask_)) continue;

        CollisionInfo info;
        if (checkCollision(other, info)) {
            currentCollisions_.push_back(info);
            resolveCollision(info);
        }
    }

    // Apply avoidance forces if enabled
    if (avoidanceEnabled_) {
        Vector2f avoidanceForce = calculateAvoidanceForce(nearbyShips);
        if (avoidanceForce.lengthSquared() > 0.0f) {
            ship_->setVelocity(ship_->getVelocity() + avoidanceForce * deltaTime);
        }
    }
}

void CollisionComponent::resolveCollision(CollisionInfo& info) {
    // Calculate relative velocity
    Vector2f relativeVelocity = ship_->getVelocity() - info.otherShip->getVelocity();

    // Calculate impulse
    float velocityAlongNormal = relativeVelocity.dot(info.normal);

    // Don't resolve if objects are moving apart
    if (velocityAlongNormal > 0.0f) return;

    // Calculate restitution
    float restitution = std::min(restitution_, info.otherShip->getComponent<CollisionComponent>()->restitution_);

    // Calculate impulse scalar
    float impulseScalar = -(1.0f + restitution) * velocityAlongNormal;
    impulseScalar /= 1.0f / ship_->getMass() + 1.0f / info.otherShip->getMass();

    // Apply impulse
    Vector2f impulse = info.normal * impulseScalar;
    ship_->setVelocity(ship_->getVelocity() + impulse / ship_->getMass());
    info.otherShip->setVelocity(info.otherShip->getVelocity() - impulse / info.otherShip->getMass());

    // Store impulse for callbacks
    info.impulse = impulseScalar;
}

void CollisionComponent::updateCollisionCallbacks() {
    // Find new collisions
    for (const auto& current : currentCollisions_) {
        auto it = std::find_if(previousCollisions_.begin(), previousCollisions_.end(),
            [&current](const CollisionInfo& prev) {
                return prev.otherShip == current.otherShip;
            });

        if (it == previousCollisions_.end()) {
            handleCollisionStart(current);
        } else {
            handleCollision(current);
        }
    }

    // Find ended collisions
    for (const auto& previous : previousCollisions_) {
        auto it = std::find_if(currentCollisions_.begin(), currentCollisions_.end(),
            [&previous](const CollisionInfo& current) {
                return current.otherShip == previous.otherShip;
            });

        if (it == currentCollisions_.end()) {
            handleCollisionEnd(previous);
        }
    }
}

bool CollisionComponent::checkCollision(const Ship* other, CollisionInfo& info) const {
    if (!collisionShape_ || !other->getComponent<CollisionComponent>()->collisionShape_) {
        return false;
    }

    const auto& thisTransform = ship_->getTransform();
    const auto& otherTransform = other->getTransform();

    return collisionShape_->testCollision(
        thisTransform,
        other->getComponent<CollisionComponent>()->collisionShape_.get(),
        otherTransform,
        info
    );
}

bool CollisionComponent::checkPointCollision(const Vector2f& point, CollisionInfo& info) const {
    if (!collisionShape_) return false;

    const auto& transform = ship_->getTransform();
    return collisionShape_->testPoint(transform, point, info);
}

Vector2f CollisionComponent::calculateAvoidanceForce(const std::vector<Ship*>& nearbyShips) const {
    Vector2f avoidanceForce(0.0f, 0.0f);
    int count = 0;

    for (Ship* other : nearbyShips) {
        if (other == ship_) continue;

        Vector2f toOther = other->getTransform().position - ship_->getTransform().position;
        float distance = toOther.length();

        if (distance < avoidanceRadius_) {
            // Calculate repulsion force
            float strength = 1.0f - (distance / avoidanceRadius_);
            Vector2f force = toOther.normalized() * strength;

            // Weight by relative velocity
            Vector2f relativeVelocity = other->getVelocity() - ship_->getVelocity();
            float velocityFactor = relativeVelocity.length() / ship_->getMaxSpeed();

            avoidanceForce += force * velocityFactor;
            count++;
        }
    }

    if (count > 0) {
        avoidanceForce /= static_cast<float>(count);
    }

    return avoidanceForce;
}

// Public interface implementations
void CollisionComponent::setCollisionShape(std::unique_ptr<physics::CollisionShape> shape) {
    collisionShape_ = std::move(shape);
}

void CollisionComponent::setCollisionLayer(physics::CollisionLayer layer) {
    layer_ = layer;
}

void CollisionComponent::setCollisionMask(uint32_t mask) {
    collisionMask_ = mask;
}

void CollisionComponent::setElasticity(float elasticity) {
    elasticity_ = std::clamp(elasticity, 0.0f, 1.0f);
}

void CollisionComponent::setFriction(float friction) {
    friction_ = std::clamp(friction, 0.0f, 1.0f);
}

void CollisionComponent::setRestitution(float restitution) {
    restitution_ = std::clamp(restitution, 0.0f, 1.0f);
}

void CollisionComponent::setOnCollision(CollisionCallback callback) {
    onCollision_ = std::move(callback);
}

void CollisionComponent::setOnCollisionStart(CollisionCallback callback) {
    onCollisionStart_ = std::move(callback);
}

void CollisionComponent::setOnCollisionEnd(CollisionCallback callback) {
    onCollisionEnd_ = std::move(callback);
}

bool CollisionComponent::isColliding() const {
    return !currentCollisions_.empty();
}

const std::vector<CollisionInfo>& CollisionComponent::getCurrentCollisions() const {
    return currentCollisions_;
}

bool CollisionComponent::testCollision(const Ship* other) const {
    CollisionInfo info;
    return checkCollision(other, info);
}

bool CollisionComponent::testCollision(const Vector2f& point) const {
    CollisionInfo info;
    return checkPointCollision(point, info);
}

void CollisionComponent::setAvoidanceEnabled(bool enabled) {
    avoidanceEnabled_ = enabled;
}

void CollisionComponent::setAvoidanceRadius(float radius) {
    avoidanceRadius_ = radius;
}

void CollisionComponent::handleCollisionStart(const CollisionInfo& info) {
    if (onCollisionStart_) {
        onCollisionStart_(info);
    }
}

void CollisionComponent::handleCollisionEnd(const CollisionInfo& info) {
    if (onCollisionEnd_) {
        onCollisionEnd_(info);
    }
}

void CollisionComponent::handleCollision(const CollisionInfo& info) {
    if (onCollision_) {
        onCollision_(info);
    }
}

} // namespace game
} // namespace void_contingency
```

## Key Concepts Explained

### Collision Detection

1. **Broad Phase**:

   - Spatial partitioning
   - AABB tests
   - Object culling

2. **Narrow Phase**:

   - Precise collision tests
   - Shape intersection
   - Contact points

3. **Continuous Detection**:
   - Sweep tests
   - Tunneling prevention
   - Time of impact

### Collision Response

1. **Impulse Resolution**:

   - Conservation of momentum
   - Elasticity handling
   - Friction forces

2. **Position Correction**:

   - Overlap resolution
   - Stability maintenance
   - Stacking behavior

3. **Energy Conservation**:
   - Velocity updates
   - Angular momentum
   - Restitution

### Collision Shapes

1. **Circle Colliders**:

   - Simple and efficient
   - Radius-based tests
   - Uniform response

2. **Box Colliders**:

   - Oriented boxes
   - Corner handling
   - Edge collisions

3. **Compound Shapes**:
   - Multiple primitives
   - Hierarchical testing
   - Complex objects

### Collision Layers

1. **Layer Matrix**:

   - Collision masks
   - Group filtering
   - Layer priorities

2. **Trigger Zones**:

   - Non-physical collisions
   - Event triggers
   - Sensor regions

3. **Layer Rules**:
   - Collision permissions
   - Response types
   - Layer inheritance

## Common Pitfalls

1. **Performance Issues**:

   - Too many collision checks
   - Inefficient broad phase
   - Complex shapes

2. **Stability Problems**:

   - Stacking instability
   - Tunneling
   - Jittering

3. **Edge Cases**:

   - High velocity collisions
   - Resting contacts
   - Corner cases

4. **Memory Management**:
   - Contact point allocation
   - Collision pair tracking
   - Event handling

## Best Practices

1. **Optimization**:

   - Use simple collision shapes
   - Implement spatial partitioning
   - Batch collision checks

2. **Stability**:

   - Implement position correction
   - Use continuous collision detection
   - Handle resting contacts

3. **Debugging**:

   - Visualize collision shapes
   - Log collision events
   - Monitor performance

4. **Architecture**:
   - Separate physics from graphics
   - Use event-driven design
   - Maintain clean interfaces

## Testing

Create test file `tests/unit/game/ship/components/CollisionComponentTest.cpp`:

```cpp
TEST(CollisionComponentTest, BasicCollision) {
    Ship ship1("Ship 1");
    Ship ship2("Ship 2");

    auto collision1 = std::make_unique<CollisionComponent>();
    auto collision2 = std::make_unique<CollisionComponent>();

    // Test collision detection
    ship2.setPosition(Vector2f(10.0f, 0.0f));  // Close to ship1
    EXPECT_TRUE(collision1->testCollision(&ship2));

    // Test collision response
    collision1->update(1.0f);
    EXPECT_GT(ship1.getVelocity().length(), 0.0f);
}

TEST(CollisionComponentTest, CollisionLayers) {
    Ship ship1("Ship 1");
    Ship ship2("Ship 2");

    auto collision1 = std::make_unique<CollisionComponent>();
    auto collision2 = std::make_unique<CollisionComponent>();

    // Set different layers
    collision1->setCollisionLayer(CollisionLayer::Player);
    collision2->setCollisionLayer(CollisionLayer::Enemy);

    // Test layer-based collision
    EXPECT_TRUE(collision1->canCollideWith(collision2.get()));
}
```

## Next Steps

1. Add advanced collision features:

   - Soft body collisions
   - Fluid dynamics
   - Destructible objects

2. Implement collision optimization
3. Add collision effects system
4. Create collision debugging tools

## Additional Resources

- [Ship Visualization Tutorial](./visualization.md)
- [Movement System Tutorial](./movement_system.md)
- [Physics Programming Guide](../guides/physics_programming.md)
- [Collision System Documentation](../physics/collision_system.md)
