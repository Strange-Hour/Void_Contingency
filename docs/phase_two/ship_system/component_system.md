# Implementing Ship Component System

## Navigation

- Previous: [Basic Ship Class Tutorial](./basic_ship_class.md)
- Next: [Movement System Tutorial](./movement_system.md)

## What We're Building

We're implementing a component system that allows ships to have modular functionality through attachable components. This is a crucial game development pattern that makes our ships highly customizable and maintainable.

## Game Development Concepts Covered

1. **Component-Based Architecture**: A deeper dive into how components work in games
2. **Type Safety**: Using C++'s type system to ensure components are handled correctly
3. **Runtime Polymorphism**: Using virtual functions for component behavior
4. **Resource Management**: Managing component lifecycles
5. **Event Systems**: How components can communicate with each other
6. **Dependency Management**: Handling relationships between components
7. **Design Patterns**: Implementation of the Component Pattern

## Why This Implementation?

1. **Flexibility**: Components can be added/removed at runtime
2. **Reusability**: Components can be shared between different types of ships
3. **Separation of Concerns**: Each component handles its own specific functionality
4. **Scalability**: Easy to add new ship features by creating new components
5. **Maintainability**: Components can be modified independently

## Implementation Steps

### 1. Create Component Types

Create `include/game/ship/components/ComponentTypes.hpp`:

```cpp
#pragma once

namespace void_contingency {
namespace game {

enum class ComponentType {
    Engine,
    Weapon,
    Shield,
    Power,
    Hull,
    Sensor
};

} // namespace game
} // namespace void_contingency
```

### 2. Create Base Ship Component

Create `include/game/ship/components/ShipComponent.hpp`:

```cpp
#pragma once

#include "core/Component.hpp"
#include "game/ship/Ship.hpp"
#include "ComponentTypes.hpp"

namespace void_contingency {
namespace game {

class ShipComponent : public core::Component {
public:
    ShipComponent(ComponentType type);
    virtual ~ShipComponent() = default;

    // Component interface
    void setShip(Ship* ship) { ship_ = ship; }
    Ship* getShip() const { return ship_; }
    ComponentType getType() const { return type_; }

    // Component lifecycle
    virtual void initialize() = 0;
    virtual void shutdown() = 0;
    virtual void update(float deltaTime) override = 0;

protected:
    Ship* ship_{nullptr};
    ComponentType type_;
};

} // namespace game
} // namespace void_contingency
```

### 3. Implement Base Ship Component

Create `src/game/ship/components/ShipComponent.cpp`:

```cpp
#include "game/ship/components/ShipComponent.hpp"

namespace void_contingency {
namespace game {

ShipComponent::ShipComponent(ComponentType type)
    : type_(type)
{
}

} // namespace game
} // namespace void_contingency
```

### 4. Create Example Components

#### Engine Component

Create `include/game/ship/components/EngineComponent.hpp`:

```cpp
#pragma once

#include "ShipComponent.hpp"

namespace void_contingency {
namespace game {

class EngineComponent : public ShipComponent {
public:
    EngineComponent();

    void initialize() override;
    void shutdown() override;
    void update(float deltaTime) override;

    void setThrust(float thrust);
    float getThrust() const;
    float getMaxThrust() const;

private:
    float thrust_{0.0f};
    float maxThrust_{100.0f};
    float efficiency_{0.8f};
};

} // namespace game
} // namespace void_contingency
```

Create `src/game/ship/components/EngineComponent.cpp`:

```cpp
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
```

## Key Concepts Explained

### Component Architecture

- Components are self-contained units of functionality
- Each component has a specific responsibility
- Components can communicate through events
- Components can depend on other components

### Type Safety

- Using enum class for component types
- Template methods for type-safe component access
- Runtime type checking with dynamic_cast
- Compile-time type checking with static_assert

### Component Lifecycle

1. **Creation**: Component is instantiated and added to ship
2. **Initialization**: Component sets up its initial state
3. **Update**: Component runs its logic each frame
4. **Shutdown**: Component cleans up resources
5. **Destruction**: Component is removed from ship

### Component Communication

1. **Direct Access**: Components can access each other through the ship
2. **Event System**: Components can subscribe to and emit events
3. **Message Passing**: Components can send messages to each other
4. **Dependency Injection**: Components can be injected into other components

## Common Pitfalls

1. **Circular Dependencies**:

   - Components depending on each other cyclically
   - Solution: Use events or message passing instead

2. **Order of Operations**:

   - Components need to update in specific order
   - Solution: Implement priority system or explicit ordering

3. **Resource Management**:

   - Memory leaks from improper cleanup
   - Solution: Use RAII and smart pointers

4. **Type Safety**:
   - Unsafe component casting
   - Solution: Use type-safe accessors and proper error handling

## Best Practices

1. **Single Responsibility**:

   - Each component should do one thing well
   - Split complex components into smaller ones

2. **Dependency Management**:

   - Clearly define component dependencies
   - Use dependency injection where possible
   - Avoid tight coupling between components

3. **Error Handling**:

   - Validate component operations
   - Handle missing dependencies gracefully
   - Provide meaningful error messages

4. **Performance**:
   - Cache component lookups where possible
   - Minimize component communication overhead
   - Use appropriate data structures for component storage

## Testing

Create test files for components:

```cpp
// Example test for EngineComponent
TEST(EngineComponentTest, BasicFunctionality) {
    Ship ship("Test Ship");
    auto engine = std::make_unique<EngineComponent>();

    engine->setThrust(50.0f);
    EXPECT_EQ(engine->getThrust(), 50.0f);

    engine->update(1.0f);
    EXPECT_GT(ship.getVelocity().length(), 0.0f);
}
```

## Next Steps

1. Implement more specialized components:

   - Weapons systems
   - Shield systems
   - Power management
   - Sensor systems

2. Add component configuration system
3. Implement component serialization
4. Add component debugging tools

## Additional Resources

- [Basic Ship Class Tutorial](./basic_ship_class.md)
- [Movement System Tutorial](./movement_system.md)
- [Ship Visualization Tutorial](./visualization.md)
- [Collision System Tutorial](./collision_system.md)
