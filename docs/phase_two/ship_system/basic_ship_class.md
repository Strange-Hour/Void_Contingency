# Implementing Basic Ship Class

## Navigation

- Previous: [Phase 1 Documentation](../phase_one/README.md)
- Next: [Component System Tutorial](./component_system.md)

## What We're Building

We're creating the fundamental building block of the game's ship system - the `Ship` class. This is the base class that will represent any ship in the game.

## Game Development Concepts Covered

1. **Entity Management**: How to create and manage game entities (ships in this case)
2. **Component-Based Architecture**: A design pattern commonly used in games where functionality is split into reusable components
3. **State Management**: Handling entity properties and state changes
4. **Transform Systems**: Managing position, rotation, and scale of game objects
5. **Delta Time**: Using time-based updates for smooth movement
6. **Memory Management**: Using smart pointers for safe memory handling
7. **Encapsulation**: Proper OOP principles in game development

## Why This Implementation?

The tutorial uses this approach because:

1. **Modularity**: The component-based system makes it easy to add or remove features
2. **Maintainability**: Clear separation of concerns makes the code easier to maintain
3. **Flexibility**: The design allows for easy extension of ship capabilities
4. **Performance**: Using modern C++ features for efficient memory management
5. **Testability**: The structure makes it easy to write unit tests

## Implementation Steps

### 1. Create the Ship Header File

Create a new file `include/game/ship/Ship.hpp`:

```cpp
#pragma once

#include <memory>
#include <vector>
#include <string>
#include "core/Component.hpp"
#include "core/Transform.hpp"

namespace void_contingency {
namespace game {

class Ship {
public:
    Ship(const std::string& name);
    ~Ship() = default;

    // Basic properties
    const std::string& getName() const { return name_; }
    float getHealth() const { return health_; }
    float getMaxHealth() const { return maxHealth_; }
    const Transform& getTransform() const { return transform_; }

    // Movement
    void setVelocity(const Vector2f& velocity) { velocity_ = velocity; }
    const Vector2f& getVelocity() const { return velocity_; }

    // Component management
    void addComponent(std::unique_ptr<Component> component);
    template<typename T>
    T* getComponent() const;

    // State management
    void update(float deltaTime);
    void damage(float amount);
    void heal(float amount);

private:
    std::string name_;
    float health_;
    float maxHealth_;
    Transform transform_;
    Vector2f velocity_;
    std::vector<std::unique_ptr<Component>> components_;
};

} // namespace game
} // namespace void_contingency
```

### 2. Implement the Ship Class

Create `src/game/ship/Ship.cpp`:

```cpp
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
```

### 3. Create the Transform Class

Create `include/core/Transform.hpp`:

```cpp
#pragma once

#include "Vector2f.hpp"

namespace void_contingency {
namespace core {

struct Transform {
    Vector2f position{0.0f, 0.0f};
    float rotation{0.0f};
    Vector2f scale{1.0f, 1.0f};
};

} // namespace core
} // namespace void_contingency
```

### 4. Create the Component Base Class

Create `include/core/Component.hpp`:

```cpp
#pragma once

namespace void_contingency {
namespace core {

class Component {
public:
    virtual ~Component() = default;
    virtual void update(float deltaTime) = 0;
};

} // namespace core
} // namespace void_contingency
```

## Key Concepts Explained

### Component-Based Architecture

- Components are modular pieces of functionality that can be attached to ships
- Each component handles a specific aspect (movement, weapons, shields, etc.)
- Components can be added/removed dynamically
- Makes it easy to create different types of ships by combining components

### Transform System

- Handles position, rotation, and scale of ships
- Uses 2D vectors for position and scale
- Rotation is stored in degrees
- Provides methods for local and world space transformations

### State Management

- Health system with current and max health
- Damage and healing methods with proper bounds checking
- Component state tracking
- Transform state updates

### Memory Management

- Smart pointers (`std::unique_ptr`) for automatic memory cleanup
- Component ownership handled by the ship class
- Safe component access through templated methods
- Proper cleanup in destructors

## Common Pitfalls

1. **Memory Management**:

   - Always use smart pointers for components
   - Be careful with component ownership
   - Clean up resources properly

2. **Update Order**:

   - Components should be updated in a specific order
   - Transform updates should happen after movement
   - State changes should be processed before rendering

3. **State Synchronization**:
   - Keep ship state consistent across all components
   - Handle edge cases (zero health, max health)
   - Validate state changes

## Best Practices

1. Use smart pointers for component management
2. Keep the ship class focused on core functionality
3. Use the component system for extensibility
4. Implement proper error handling
5. Add logging for debugging

## Testing

Create a test file `tests/unit/game/ship/ShipTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "game/ship/Ship.hpp"

using namespace void_contingency::game;

TEST(ShipTest, BasicProperties) {
    Ship ship("Test Ship");

    EXPECT_EQ(ship.getName(), "Test Ship");
    EXPECT_EQ(ship.getHealth(), 100.0f);
    EXPECT_EQ(ship.getMaxHealth(), 100.0f);
}

TEST(ShipTest, Movement) {
    Ship ship("Test Ship");
    Vector2f velocity(1.0f, 2.0f);

    ship.setVelocity(velocity);
    EXPECT_EQ(ship.getVelocity(), velocity);
}

TEST(ShipTest, DamageAndHeal) {
    Ship ship("Test Ship");

    ship.damage(30.0f);
    EXPECT_EQ(ship.getHealth(), 70.0f);

    ship.heal(20.0f);
    EXPECT_EQ(ship.getHealth(), 90.0f);

    ship.damage(100.0f);
    EXPECT_EQ(ship.getHealth(), 0.0f);

    ship.heal(100.0f);
    EXPECT_EQ(ship.getHealth(), 100.0f);
}
```

## Next Steps

1. Implement specific ship components (engines, weapons, shields)
2. Add collision detection
3. Implement ship visualization
4. Add ship configuration system

## Additional Resources

- [Phase 1 Documentation](../phase_one/README.md)
- [Component System Tutorial](./component_system.md)
- [Movement System Tutorial](./movement_system.md)
- [Ship Visualization Tutorial](./visualization.md)
- [Collision System Tutorial](./collision_system.md)
