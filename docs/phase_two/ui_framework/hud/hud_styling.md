# Implementing HUD Styling

## Navigation

- Previous: [HUD Input Handling Tutorial](./hud_input_handling.md)
- Next: [HUD Persistence Tutorial](./hud_persistence.md)

This tutorial will guide you through implementing a styling system for the HUD in "Void Contingency", focusing on themes, visual effects, and customization options.

## Overview

The HUD styling system will:

- Support theme-based styling
- Enable visual effects and animations
- Provide customization options
- Handle style inheritance
- Support dynamic style updates
- Enable style presets

## Implementation Steps

### 1. Create HUD Style Types

Create `include/ui/hud/HUDStyle.hpp`:

```cpp
#pragma once

#include "ui/hud/HUDTypes.hpp"
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include <chrono>

namespace void_contingency {
namespace ui {

struct ColorScheme {
    Color primary{1.0f, 1.0f, 1.0f, 1.0f};
    Color secondary{0.5f, 0.5f, 0.5f, 1.0f};
    Color accent{0.0f, 0.5f, 1.0f, 1.0f};
    Color background{0.0f, 0.0f, 0.0f, 0.8f};
    Color text{1.0f, 1.0f, 1.0f, 1.0f};
    Color border{0.5f, 0.5f, 0.5f, 1.0f};
};

struct Typography {
    std::string fontFamily{"Arial"};
    float fontSize{16.0f};
    float lineHeight{1.2f};
    float letterSpacing{0.0f};
    bool bold{false};
    bool italic{false};
    bool underline{false};
};

struct Layout {
    float padding{8.0f};
    float margin{4.0f};
    float borderWidth{1.0f};
    float borderRadius{4.0f};
    float spacing{8.0f};
    bool autoSize{true};
};

struct VisualEffect {
    bool shadow{false};
    float shadowBlur{4.0f};
    Color shadowColor{0.0f, 0.0f, 0.0f, 0.5f};
    bool glow{false};
    float glowIntensity{1.0f};
    Color glowColor{1.0f, 1.0f, 1.0f, 1.0f};
    bool blur{false};
    float blurAmount{0.0f};
};

struct HUDStyle {
    std::string id;
    std::string name;
    std::string description;
    ColorScheme colors;
    Typography typography;
    Layout layout;
    VisualEffect effects;
    std::unordered_map<std::string, std::string> customProperties;
    std::vector<std::string> inheritedStyles;
    bool isEnabled{true};
};

using StyleCallback = std::function<void(const HUDStyle&)>;

} // namespace ui
} // namespace void_contingency
```

### 2. Create HUD Style Manager

Create `include/ui/hud/HUDStyleManager.hpp`:

```cpp
#pragma once

#include "ui/hud/HUDStyle.hpp"
#include <memory>
#include <vector>
#include <unordered_map>

namespace void_contingency {
namespace ui {

class HUDStyleManager {
public:
    static HUDStyleManager& getInstance();

    // Style management
    void addStyle(const HUDStyle& style);
    void removeStyle(const std::string& id);
    void updateStyle(const std::string& id, const HUDStyle& style);
    void enableStyle(const std::string& id);
    void disableStyle(const std::string& id);

    // Style control
    void update(float deltaTime);
    void applyStyle(const std::string& id);
    void resetStyles();

    // Style query
    HUDStyle getStyle(const std::string& id) const;
    std::vector<HUDStyle> getAllStyles() const;
    bool hasStyle(const std::string& id) const;
    bool isStyleEnabled(const std::string& id) const;

    // Style subscription
    void subscribeToStyle(const std::string& id, StyleCallback callback);
    void unsubscribeFromStyle(const std::string& id, StyleCallback callback);

private:
    HUDStyleManager() = default;
    ~HUDStyleManager() = default;
    HUDStyleManager(const HUDStyleManager&) = delete;
    HUDStyleManager& operator=(const HUDStyleManager&) = delete;

    // Style storage
    std::unordered_map<std::string, HUDStyle> styles_;
    std::unordered_map<std::string, std::vector<StyleCallback>> callbacks_;

    // Helper methods
    void updateStyleEffects(HUDStyle& style, float deltaTime);
    void notifyCallbacks(const std::string& id, const HUDStyle& style);
    void applyStyleInheritance(HUDStyle& style);
    void validateStyle(const HUDStyle& style);
    void updateStyleProperties(HUDStyle& style);
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create HUD Style View

Create `include/ui/hud/HUDStyleView.hpp`:

```cpp
#pragma once

#include "ui/hud/HUDStyle.hpp"
#include "ui/components/Component.hpp"
#include <memory>

namespace void_contingency {
namespace ui {

class HUDStyleView : public Component {
public:
    HUDStyleView();
    ~HUDStyleView() override;

    // View control
    void setStyle(const HUDStyle& style);
    void setColorScheme(const ColorScheme& colors);
    void setTypography(const Typography& typography);

    // Interactive features
    void setInteractive(bool interactive) { interactive_ = interactive; }
    bool isInteractive() const { return interactive_; }

    // View customization
    void setShowColors(bool show) { showColors_ = show; }
    void setShowTypography(bool show) { showTypography_ = show; }
    void setShowLayout(bool show) { showLayout_ = show; }
    void setShowEffects(bool show) { showEffects_ = show; }

protected:
    void render(Renderer& renderer) override;
    void handleInput(const InputEvent& event) override;
    void update() override;

private:
    // View state
    HUDStyle currentStyle_;
    ColorScheme currentColors_;
    Typography currentTypography_;
    bool interactive_{false};
    bool showColors_{true};
    bool showTypography_{true};
    bool showLayout_{true};
    bool showEffects_{true};

    // Helper methods
    void renderColors(Renderer& renderer);
    void renderTypography(Renderer& renderer);
    void renderLayout(Renderer& renderer);
    void renderEffects(Renderer& renderer);
    void handleStyleInput(const InputEvent& event);
    void updateVisualization();
};

} // namespace ui
} // namespace void_contingency
```

### 4. Create Style Tests

Create `tests/unit/ui/hud/StyleTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/hud/HUDStyle.hpp"
#include "ui/hud/HUDStyleManager.hpp"
#include "ui/hud/HUDStyleView.hpp"

using namespace void_contingency::ui;

TEST(HUDStyleManagerTest, StyleManagement) {
    auto& manager = HUDStyleManager::getInstance();

    // Create test style
    HUDStyle style;
    style.id = "test_style";
    style.name = "Test Style";
    style.colors.primary = Color(1.0f, 0.0f, 0.0f, 1.0f);
    style.typography.fontSize = 20.0f;
    style.layout.padding = 16.0f;
    style.effects.shadow = true;

    // Add style
    manager.addStyle(style);

    // Verify style
    EXPECT_TRUE(manager.hasStyle("test_style"));
    EXPECT_EQ(manager.getStyle("test_style").name, "Test Style");

    // Update style
    style.colors.primary = Color(0.0f, 1.0f, 0.0f, 1.0f);
    manager.updateStyle("test_style", style);
    EXPECT_EQ(manager.getStyle("test_style").colors.primary.g, 1.0f);

    // Disable style
    manager.disableStyle("test_style");
    EXPECT_FALSE(manager.isStyleEnabled("test_style"));
}

TEST(HUDStyleViewTest, ViewRendering) {
    HUDStyleView view;

    // Set test style
    HUDStyle style;
    style.id = "test_style";
    style.name = "Test Style";
    view.setStyle(style);

    // Set test colors
    ColorScheme colors;
    colors.primary = Color(1.0f, 0.0f, 0.0f, 1.0f);
    view.setColorScheme(colors);

    // Verify view state
    EXPECT_FALSE(view.isInteractive());
    view.setInteractive(true);
    EXPECT_TRUE(view.isInteractive());
}
```

## Next Steps

1. Implement HUD persistence
2. Add HUD debugging tools
3. Implement HUD analytics
4. Add HUD optimization

## Common Pitfalls

1. **Style Inheritance**: Handle style inheritance properly
2. **Performance**: Optimize style updates for many elements
3. **Resource Usage**: Control memory usage for styles
4. **Style Validation**: Validate style properties

## Best Practices

1. Use efficient style updates
2. Implement proper inheritance
3. Support style customization
4. Enable style presets
5. Provide clear feedback

## Additional Resources

- [UI Component System Tutorial](../core/ui_component_system.md)
- [Layout Management Tutorial](../core/layout_management.md)
- [Styling System Tutorial](../core/styling_system.md)
- [Component Rendering Tutorial](../core/component_rendering.md)
- [Component Animations Tutorial](../components/component_animations.md)
- [Post-Processing Effects Tutorial](../components/post_processing.md)
- [Clipping and Masking Tutorial](../clipping/clipping_masking.md)
- [Nested Clipping Tutorial](../clipping/nested_clipping.md)
- [Clipping Optimizations Tutorial](../clipping/clipping_optimizations.md)
- [Clipping Debugging Tutorial](../clipping/clipping_debugging.md)
- [Clipping Serialization Tutorial](../clipping/clipping_serialization.md)
- [Clipping Compression Tutorial](../clipping/clipping_compression.md)
- [Input Handling Tutorial](../input/input_handling.md)
- [Input Gestures Tutorial](../input/input_gestures.md)
- [Gesture Recording Tutorial](../gestures/gesture_recording.md)
- [Gesture Analysis Tutorial](../gestures/gesture_analysis.md)
- [Gesture Visualization Tutorial](../gestures/gesture_visualization.md)
- [Gesture Templates Tutorial](../gestures/gesture_templates.md)
- [Gesture Training Tutorial](../gestures/gesture_training.md)
- [Gesture Export/Import Tutorial](../gestures/gesture_export_import.md)
- [Gesture Collaboration Tutorial](../gestures/gesture_collaboration.md)
- [Gesture Recognition Tutorial](../gestures/gesture_recognition.md)
- [Gesture Customization Tutorial](../gestures/gesture_customization.md)
- [Gesture Validation Tutorial](../gestures/gesture_validation.md)
- [Gesture Analytics Tutorial](../gestures/gesture_analytics.md)
- [Gesture Optimization Tutorial](../gestures/gesture_optimization.md)
- [Gesture Sharing Tutorial](../gestures/gesture_sharing.md)
- [HUD Elements Tutorial](./hud_elements.md)
- [HUD Animations Tutorial](./hud_animations.md)
- [HUD State Management Tutorial](./hud_state_management.md)
- [HUD Transitions Tutorial](./hud_transitions.md)
- [HUD Input Handling Tutorial](./hud_input_handling.md)
- [HUD Persistence Tutorial](./hud_persistence.md)
- [HUD Debugging Tutorial](./hud_debugging.md)
- [HUD Analytics Tutorial](./hud_analytics.md)
- [HUD Optimization Tutorial](./hud_optimization.md)
- [HUD Sharing Tutorial](./hud_sharing.md)
- [HUD Testing Tutorial](./hud_testing.md)
- [HUD Profiling Tutorial](./hud_profiling.md)
- [HUD Benchmarking Tutorial](./hud_benchmarking.md)
