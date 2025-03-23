# Implementing Ship Visualization

## Navigation

- Previous: [Movement System Tutorial](./movement_system.md)
- Next: [Collision System Tutorial](./collision_system.md)

## What We're Building

We're implementing a visualization system that handles all visual aspects of ships, including rendering, animations, and special effects. This system ensures ships look good and provide clear visual feedback to players.

## Game Development Concepts Covered

1. **Sprite Rendering**: Basic 2D graphics rendering
2. **Animation Systems**: Handling sprite animations
3. **Particle Effects**: Visual effects for engines, damage, etc.
4. **State-Based Visualization**: Different visuals based on ship state
5. **Visual Feedback**: Communicating game state through visuals
6. **Transform Systems**: Positioning and rotating sprites
7. **Color Management**: Using colors for visual feedback
8. **Layer Management**: Handling render order of different elements

## Why This Implementation?

1. **Modularity**: Separation of visual concerns from game logic
2. **Flexibility**: Easy to add new visual effects and animations
3. **Performance**: Efficient rendering through proper sprite management
4. **Feedback**: Clear visual communication of ship state
5. **Extensibility**: Easy to add new visual features

## Implementation Steps

### 1. Create Visualization Component

Create `include/game/ship/components/VisualizationComponent.hpp`:

```cpp
#pragma once

#include "ShipComponent.hpp"
#include "graphics/Sprite.hpp"
#include "graphics/Animation.hpp"
#include <unordered_map>

namespace void_contingency {
namespace game {

enum class ShipVisualState {
    Normal,
    Damaged,
    Critical,
    Destroyed
};

class VisualizationComponent : public ShipComponent {
public:
    VisualizationComponent();

    void initialize() override;
    void shutdown() override;
    void update(float deltaTime) override;

    // Visual state management
    void setVisualState(ShipVisualState state);
    void setDamageLevel(float damageLevel);

    // Sprite management
    void setHullSprite(const std::string& spriteId);
    void addComponentSprite(const std::string& componentId, const std::string& spriteId);
    void removeComponentSprite(const std::string& componentId);

    // Animation control
    void playAnimation(const std::string& animationId);
    void stopAnimation(const std::string& animationId);
    void setAnimationSpeed(const std::string& animationId, float speed);

    // Visual effects
    void addTrailEffect(const Vector2f& offset, const Color& color);
    void addDamageEffect(const Vector2f& position);
    void addExplosionEffect();

private:
    void updateVisualState();
    void updateAnimations(float deltaTime);
    void updateEffects(float deltaTime);
    void renderShip();
    void renderComponents();
    void renderEffects();

    ShipVisualState currentState_{ShipVisualState::Normal};
    float damageLevel_{0.0f};

    std::unique_ptr<graphics::Sprite> hullSprite_;
    std::unordered_map<std::string, std::unique_ptr<graphics::Sprite>> componentSprites_;
    std::unordered_map<std::string, std::unique_ptr<graphics::Animation>> animations_;

    struct TrailEffect {
        Vector2f offset;
        Color color;
        float lifetime;
        float currentTime;
    };

    struct DamageEffect {
        Vector2f position;
        float lifetime;
        float currentTime;
    };

    std::vector<TrailEffect> trailEffects_;
    std::vector<DamageEffect> damageEffects_;
    bool hasExplosionEffect_{false};
};

} // namespace game
} // namespace void_contingency
```

### 2. Implement Visualization Component

Create `src/game/ship/components/VisualizationComponent.cpp`:

```cpp
#include "game/ship/components/VisualizationComponent.hpp"
#include "graphics/Renderer.hpp"
#include <algorithm>

namespace void_contingency {
namespace game {

VisualizationComponent::VisualizationComponent()
    : ShipComponent(ComponentType::Visualization)
{
}

void VisualizationComponent::initialize() {
    // Initialize visualization systems
}

void VisualizationComponent::shutdown() {
    // Clean up visualization systems
}

void VisualizationComponent::update(float deltaTime) {
    if (!ship_) return;

    updateVisualState();
    updateAnimations(deltaTime);
    updateEffects(deltaTime);
    renderShip();
    renderComponents();
    renderEffects();
}

void VisualizationComponent::updateVisualState() {
    float healthPercent = ship_->getHealth() / ship_->getMaxHealth();

    if (healthPercent <= 0.0f) {
        currentState_ = ShipVisualState::Destroyed;
    } else if (healthPercent <= 0.3f) {
        currentState_ = ShipVisualState::Critical;
    } else if (healthPercent <= 0.7f) {
        currentState_ = ShipVisualState::Damaged;
    } else {
        currentState_ = ShipVisualState::Normal;
    }
}

void VisualizationComponent::updateAnimations(float deltaTime) {
    for (auto& [id, animation] : animations_) {
        animation->update(deltaTime);
    }
}

void VisualizationComponent::updateEffects(float deltaTime) {
    // Update trail effects
    auto it = trailEffects_.begin();
    while (it != trailEffects_.end()) {
        it->currentTime += deltaTime;
        if (it->currentTime >= it->lifetime) {
            it = trailEffects_.erase(it);
        } else {
            ++it;
        }
    }

    // Update damage effects
    auto damageIt = damageEffects_.begin();
    while (damageIt != damageEffects_.end()) {
        damageIt->currentTime += deltaTime;
        if (damageIt->currentTime >= damageIt->lifetime) {
            damageIt = damageEffects_.erase(damageIt);
        } else {
            ++damageIt;
        }
    }
}

void VisualizationComponent::renderShip() {
    if (!hullSprite_) return;

    const auto& transform = ship_->getTransform();

    // Apply damage tint based on state
    Color tint = Color::White;
    switch (currentState_) {
        case ShipVisualState::Damaged:
            tint = Color(1.0f, 0.8f, 0.8f);
            break;
        case ShipVisualState::Critical:
            tint = Color(1.0f, 0.5f, 0.5f);
            break;
        case ShipVisualState::Destroyed:
            tint = Color(0.5f, 0.5f, 0.5f);
            break;
        default:
            break;
    }

    hullSprite_->render(transform.position, transform.rotation, transform.scale, tint);
}

void VisualizationComponent::renderComponents() {
    const auto& transform = ship_->getTransform();

    for (const auto& [id, sprite] : componentSprites_) {
        // Apply component-specific transformations
        Vector2f offset = getComponentOffset(id);
        Vector2f position = transform.position + offset;

        sprite->render(position, transform.rotation, transform.scale);
    }
}

void VisualizationComponent::renderEffects() {
    const auto& transform = ship_->getTransform();

    // Render trail effects
    for (const auto& trail : trailEffects_) {
        float alpha = 1.0f - (trail.currentTime / trail.lifetime);
        Color color = trail.color;
        color.a = alpha;

        Vector2f position = transform.position + trail.offset;
        renderTrailEffect(position, color);
    }

    // Render damage effects
    for (const auto& damage : damageEffects_) {
        float alpha = 1.0f - (damage.currentTime / damage.lifetime);
        renderDamageEffect(damage.position, alpha);
    }

    // Render explosion effect if active
    if (hasExplosionEffect_) {
        renderExplosionEffect(transform.position);
    }
}

// Public interface implementations
void VisualizationComponent::setVisualState(ShipVisualState state) {
    currentState_ = state;
}

void VisualizationComponent::setDamageLevel(float damageLevel) {
    damageLevel_ = std::clamp(damageLevel, 0.0f, 1.0f);
}

void VisualizationComponent::setHullSprite(const std::string& spriteId) {
    hullSprite_ = std::make_unique<graphics::Sprite>(spriteId);
}

void VisualizationComponent::addComponentSprite(const std::string& componentId, const std::string& spriteId) {
    componentSprites_[componentId] = std::make_unique<graphics::Sprite>(spriteId);
}

void VisualizationComponent::removeComponentSprite(const std::string& componentId) {
    componentSprites_.erase(componentId);
}

void VisualizationComponent::playAnimation(const std::string& animationId) {
    auto it = animations_.find(animationId);
    if (it != animations_.end()) {
        it->second->play();
    }
}

void VisualizationComponent::stopAnimation(const std::string& animationId) {
    auto it = animations_.find(animationId);
    if (it != animations_.end()) {
        it->second->stop();
    }
}

void VisualizationComponent::setAnimationSpeed(const std::string& animationId, float speed) {
    auto it = animations_.find(animationId);
    if (it != animations_.end()) {
        it->second->setSpeed(speed);
    }
}

void VisualizationComponent::addTrailEffect(const Vector2f& offset, const Color& color) {
    TrailEffect effect{
        offset,
        color,
        1.0f,  // lifetime
        0.0f   // current time
    };
    trailEffects_.push_back(effect);
}

void VisualizationComponent::addDamageEffect(const Vector2f& position) {
    DamageEffect effect{
        position,
        0.5f,  // lifetime
        0.0f   // current time
    };
    damageEffects_.push_back(effect);
}

void VisualizationComponent::addExplosionEffect() {
    hasExplosionEffect_ = true;
}

} // namespace game
} // namespace void_contingency
```

## Key Concepts Explained

### Sprite Management

1. **Hull Sprites**:

   - Base ship appearance
   - State-based variations
   - Damage visualization

2. **Component Sprites**:

   - Attached component visuals
   - Dynamic positioning
   - Layer ordering

3. **Animation Sprites**:
   - Movement effects
   - State transitions
   - Special effects

### Visual States

1. **Normal State**:

   - Default ship appearance
   - Standard animations
   - Regular effects

2. **Damaged State**:

   - Visual damage indicators
   - Smoke/fire effects
   - Color tinting

3. **Critical State**:

   - Emergency visual effects
   - Warning indicators
   - Intense animations

4. **Destroyed State**:
   - Destruction animation
   - Debris particles
   - Fade out effects

### Special Effects

1. **Trail Effects**:

   - Engine exhaust
   - Movement trails
   - Particle systems

2. **Damage Effects**:

   - Impact visuals
   - Damage indicators
   - Repair animations

3. **Status Effects**:
   - Shield visualization
   - Power-up effects
   - Status indicators

### Animation System

1. **Sprite Animation**:

   - Frame-based animation
   - Smooth transitions
   - Animation blending

2. **Particle Systems**:

   - Dynamic particles
   - Effect composition
   - Performance optimization

3. **State Transitions**:
   - Smooth state changes
   - Visual interpolation
   - Effect coordination

## Common Pitfalls

1. **Performance Issues**:

   - Too many particles
   - Inefficient sprite batching
   - Unoptimized animations

2. **Visual Clarity**:

   - Overwhelming effects
   - Unclear state feedback
   - Inconsistent style

3. **Memory Management**:

   - Texture leaks
   - Unmanaged resources
   - Animation cleanup

4. **State Synchronization**:
   - Visual/logic mismatch
   - Delayed updates
   - State conflicts

## Best Practices

1. **Resource Management**:

   - Proper texture loading
   - Asset pooling
   - Memory optimization

2. **Visual Design**:

   - Clear visual hierarchy
   - Consistent style
   - Meaningful feedback

3. **Performance**:

   - Sprite batching
   - Effect limiting
   - LOD system

4. **Code Organization**:
   - Separate visual logic
   - Clean state management
   - Effect encapsulation

## Testing

Create test file `tests/unit/game/ship/components/VisualizationComponentTest.cpp`:

```cpp
TEST(VisualizationComponentTest, StateTransitions) {
    Ship ship("Test Ship");
    auto vis = std::make_unique<VisualizationComponent>();

    // Test state changes
    vis->setVisualState(ShipVisualState::Damaged);
    EXPECT_EQ(vis->getCurrentState(), ShipVisualState::Damaged);

    // Test damage visualization
    vis->setDamageLevel(0.5f);
    EXPECT_TRUE(vis->hasDamageEffects());
}

TEST(VisualizationComponentTest, EffectSystem) {
    Ship ship("Test Ship");
    auto vis = std::make_unique<VisualizationComponent>();

    // Test trail effects
    vis->addTrailEffect(Vector2f(0.0f, -10.0f), Color::Blue);
    EXPECT_GT(vis->getActiveEffects().size(), 0);
}
```

## Next Steps

1. Add advanced visual features:

   - Dynamic lighting
   - Weather effects
   - Environmental interaction

2. Implement visual customization
3. Add debug visualization
4. Create effect editor

## Additional Resources

- [Movement System Tutorial](./movement_system.md)
- [Collision System Tutorial](./collision_system.md)
- [Graphics Programming Guide](../guides/graphics_programming.md)
- [Effect System Documentation](../effects/README.md)
