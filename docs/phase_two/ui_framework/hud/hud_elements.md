# Implementing HUD Elements

## Navigation

- Previous: [HUD System Tutorial](./hud_system.md)
- Next: [HUD Animations Tutorial](./hud_animations.md)

This tutorial will guide you through implementing HUD elements for "Void Contingency", providing essential game information displays and interface components.

## Overview

The HUD system will:

- Display ship status information
- Show navigation and targeting data
- Present resource and inventory details
- Provide combat feedback
- Enable HUD customization
- Support HUD layering

## Implementation Steps

### 1. Create HUD Types

Create `include/ui/hud/HUDTypes.hpp`:

```cpp
#pragma once

#include "ui/components/Component.hpp"
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include <chrono>

namespace void_contingency {
namespace ui {

enum class HUDLayer {
    Background,
    Gameplay,
    Overlay,
    Debug
};

enum class HUDElementType {
    HealthBar,
    ShieldBar,
    EnergyBar,
    Navigation,
    Targeting,
    ResourceDisplay,
    Inventory,
    CombatFeedback,
    Minimap,
    StatusEffects
};

struct HUDLayout {
    float x{0.0f};
    float y{0.0f};
    float width{0.0f};
    float height{0.0f};
    float scale{1.0f};
    float opacity{1.0f};
    bool visible{true};
    HUDLayer layer{HUDLayer::Gameplay};
};

struct HUDMetrics {
    float health{100.0f};
    float maxHealth{100.0f};
    float shields{100.0f};
    float maxShields{100.0f};
    float energy{100.0f};
    float maxEnergy{100.0f};
    float speed{0.0f};
    float maxSpeed{100.0f};
    std::vector<std::string> statusEffects;
    std::vector<std::string> activeTargets;
    std::unordered_map<std::string, float> resources;
};

struct HUDConfig {
    bool showHealth{true};
    bool showShields{true};
    bool showEnergy{true};
    bool showNavigation{true};
    bool showTargeting{true};
    bool showResources{true};
    bool showInventory{true};
    bool showCombatFeedback{true};
    bool showMinimap{true};
    bool showStatusEffects{true};
    float opacity{1.0f};
    float scale{1.0f};
    std::string theme{"default"};
};

using HUDCallback = std::function<void(const HUDMetrics&)>;

} // namespace ui
} // namespace void_contingency
```

### 2. Create HUD Manager

Create `include/ui/hud/HUDManager.hpp`:

```cpp
#pragma once

#include "ui/hud/HUDTypes.hpp"
#include "ui/components/Component.hpp"
#include <memory>
#include <vector>

namespace void_contingency {
namespace ui {

class HUDManager {
public:
    static HUDManager& getInstance();

    // HUD control
    void updateMetrics(const HUDMetrics& metrics);
    void setConfig(const HUDConfig& config);
    void toggleElement(HUDElementType type);
    void setElementLayout(HUDElementType type, const HUDLayout& layout);

    // HUD state
    HUDMetrics getCurrentMetrics() const;
    HUDConfig getCurrentConfig() const;
    HUDLayout getElementLayout(HUDElementType type) const;
    bool isElementVisible(HUDElementType type) const;

    // HUD subscription
    void subscribeToMetrics(HUDCallback callback);
    void unsubscribeFromMetrics(HUDCallback callback);

    // HUD rendering
    void render(Renderer& renderer);
    void update(float deltaTime);

private:
    HUDManager() = default;
    ~HUDManager() = default;
    HUDManager(const HUDManager&) = delete;
    HUDManager& operator=(const HUDManager&) = delete;

    // HUD state
    HUDMetrics currentMetrics_;
    HUDConfig currentConfig_;
    std::unordered_map<HUDElementType, HUDLayout> layouts_;
    std::vector<HUDCallback> callbacks_;

    // Helper methods
    void notifyCallbacks(const HUDMetrics& metrics);
    void updateElementLayouts();
    void renderElement(Renderer& renderer, HUDElementType type);
    void validateLayout(const HUDLayout& layout);
    void applyTheme(const std::string& theme);
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create HUD Elements

Create `include/ui/hud/HUDElements.hpp`:

```cpp
#pragma once

#include "ui/hud/HUDTypes.hpp"
#include "ui/components/Component.hpp"
#include <memory>

namespace void_contingency {
namespace ui {

class HUDElement : public Component {
public:
    HUDElement(HUDElementType type);
    ~HUDElement() override;

    // Element control
    void setLayout(const HUDLayout& layout);
    void setMetrics(const HUDMetrics& metrics);
    void setConfig(const HUDConfig& config);

    // Element state
    HUDLayout getLayout() const;
    bool isVisible() const;
    HUDElementType getType() const;

    // Element customization
    void setOpacity(float opacity);
    void setScale(float scale);
    void setTheme(const std::string& theme);

protected:
    void render(Renderer& renderer) override;
    void update(float deltaTime) override;

private:
    HUDElementType type_;
    HUDLayout layout_;
    HUDMetrics metrics_;
    HUDConfig config_;

    // Helper methods
    virtual void renderElement(Renderer& renderer) = 0;
    virtual void updateElement(float deltaTime) = 0;
    void validateLayout();
    void applyTheme();
};

// Specific HUD elements
class HealthBar : public HUDElement {
public:
    HealthBar();
    void renderElement(Renderer& renderer) override;
    void updateElement(float deltaTime) override;
};

class ShieldBar : public HUDElement {
public:
    ShieldBar();
    void renderElement(Renderer& renderer) override;
    void updateElement(float deltaTime) override;
};

class NavigationDisplay : public HUDElement {
public:
    NavigationDisplay();
    void renderElement(Renderer& renderer) override;
    void updateElement(float deltaTime) override;
};

// Add more specific HUD elements as needed

} // namespace ui
} // namespace void_contingency
```

### 4. Create HUD Tests

Create `tests/unit/ui/hud/HUDTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/hud/HUDTypes.hpp"
#include "ui/hud/HUDManager.hpp"
#include "ui/hud/HUDElements.hpp"

using namespace void_contingency::ui;

TEST(HUDManagerTest, MetricsUpdate) {
    auto& manager = HUDManager::getInstance();

    // Set test metrics
    HUDMetrics metrics;
    metrics.health = 75.0f;
    metrics.shields = 50.0f;
    metrics.energy = 25.0f;
    manager.updateMetrics(metrics);

    // Verify metrics
    auto currentMetrics = manager.getCurrentMetrics();
    EXPECT_EQ(currentMetrics.health, 75.0f);
    EXPECT_EQ(currentMetrics.shields, 50.0f);
    EXPECT_EQ(currentMetrics.energy, 25.0f);

    // Test callback
    bool callbackCalled = false;
    manager.subscribeToMetrics([&callbackCalled](const HUDMetrics&) {
        callbackCalled = true;
    });

    metrics.health = 80.0f;
    manager.updateMetrics(metrics);
    EXPECT_TRUE(callbackCalled);
}

TEST(HUDElementTest, ElementRendering) {
    HealthBar healthBar;

    // Set test layout
    HUDLayout layout;
    layout.x = 10.0f;
    layout.y = 10.0f;
    layout.width = 200.0f;
    layout.height = 20.0f;
    healthBar.setLayout(layout);

    // Set test metrics
    HUDMetrics metrics;
    metrics.health = 75.0f;
    metrics.maxHealth = 100.0f;
    healthBar.setMetrics(metrics);

    // Verify element state
    EXPECT_EQ(healthBar.getType(), HUDElementType::HealthBar);
    EXPECT_TRUE(healthBar.isVisible());
    auto currentLayout = healthBar.getLayout();
    EXPECT_EQ(currentLayout.x, 10.0f);
    EXPECT_EQ(currentLayout.y, 10.0f);
}
```

## Next Steps

1. Implement menu system
2. Add input feedback system
3. Implement debug visualization tools
4. Add HUD animations

## Common Pitfalls

1. **Performance**: Optimize HUD rendering for many elements
2. **Layout Management**: Handle different screen resolutions
3. **State Updates**: Manage frequent metric updates efficiently
4. **Resource Usage**: Control memory usage for dynamic elements

## Best Practices

1. Use efficient rendering
2. Implement proper layering
3. Support customization
4. Enable dynamic updates
5. Provide clear feedback

## Additional Resources

- [HUD System Tutorial](./hud_system.md)
- [HUD Animations Tutorial](./hud_animations.md)
- [HUD State Management Tutorial](./hud_state_management.md)
- [HUD Transitions Tutorial](./hud_transitions.md)
- [HUD Input Handling Tutorial](./hud_input_handling.md)
- [HUD Styling Tutorial](./hud_styling.md)
- [HUD Persistence Tutorial](./hud_persistence.md)
- [HUD Debugging Tutorial](./hud_debugging.md)
- [HUD Analytics Tutorial](./hud_analytics.md)
- [HUD Optimization Tutorial](./hud_optimization.md)
- [HUD Sharing Tutorial](./hud_sharing.md)
- [HUD Testing Tutorial](./hud_testing.md)
- [HUD Profiling Tutorial](./hud_profiling.md)
- [HUD Benchmarking Tutorial](./hud_benchmarking.md)
