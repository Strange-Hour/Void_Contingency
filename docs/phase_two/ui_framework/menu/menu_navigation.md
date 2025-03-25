# Implementing Menu Navigation

## Navigation

- Previous: [Analytics System Tutorial](../analytics/analytics_system.md)
- Next: [Menu State Management Tutorial](./menu_state_management.md)

This tutorial will guide you through implementing menu navigation for "Void Contingency", providing a robust system for navigating through menus using keyboard, mouse, and touch input.

## Overview

The menu navigation system will:

- Support keyboard navigation
- Manage focus and selection
- Track navigation history
- Handle navigation events
- Support navigation patterns
- Enable navigation customization

## Implementation Steps

### 1. Create Menu Navigation Types

Create `include/ui/menu/MenuNavigation.hpp`:

```cpp
#pragma once

#include "ui/menu/MenuComponents.hpp"
#include <string>
#include <vector>
#include <memory>
#include <stack>
#include <unordered_map>
#include <functional>

namespace void_contingency {
namespace ui {

enum class NavigationDirection {
    Up,
    Down,
    Left,
    Right,
    Next,
    Previous,
    First,
    Last,
    Parent,
    Child,
    Tab,
    BackTab
};

enum class NavigationMode {
    Keyboard,
    Mouse,
    Touch,
    Gamepad,
    Voice,
    Gesture
};

struct NavigationHistory {
    std::string componentId;
    std::string parentId;
    NavigationMode mode;
    float timestamp;
    std::unordered_map<std::string, std::string> state;
};

class MenuNavigation {
public:
    MenuNavigation();
    ~MenuNavigation();

    // Navigation control
    void navigate(NavigationDirection direction);
    void navigateTo(const std::string& componentId);
    void navigateBack();
    void navigateForward();
    void navigateHome();
    void navigateEnd();

    // Focus management
    void setFocus(const std::string& componentId);
    void clearFocus();
    std::string getFocusedComponentId() const;
    bool hasFocus(const std::string& componentId) const;

    // Navigation history
    void pushHistory(const NavigationHistory& history);
    void popHistory();
    void clearHistory();
    const NavigationHistory& getCurrentHistory() const;
    const std::stack<NavigationHistory>& getHistory() const;

    // Navigation mode
    void setNavigationMode(NavigationMode mode);
    NavigationMode getNavigationMode() const;

    // Navigation events
    void setOnNavigate(std::function<void(const std::string&, NavigationDirection)> callback);
    void setOnFocusChange(std::function<void(const std::string&, const std::string&)> callback);
    void setOnHistoryChange(std::function<void(const NavigationHistory&)> callback);

    // Navigation patterns
    void setTabOrder(const std::vector<std::string>& order);
    void setNavigationGroup(const std::string& groupId, const std::vector<std::string>& components);
    void setNavigationSkip(const std::string& componentId, bool skip);
    void setNavigationWrap(bool wrap);

    // Navigation customization
    void setCustomNavigation(const std::string& componentId, std::function<void(NavigationDirection)> callback);
    void setNavigationDelay(float delay);
    void setNavigationRepeat(bool repeat);
    void setNavigationSpeed(float speed);

    // Navigation state
    void update();
    void reset();
    void clear();

private:
    // Navigation state
    std::string focusedComponentId_;
    NavigationMode currentMode_{NavigationMode::Keyboard};
    std::stack<NavigationHistory> history_;
    std::vector<std::string> tabOrder_;
    std::unordered_map<std::string, std::vector<std::string>> navigationGroups_;
    std::unordered_set<std::string> skippedComponents_;
    std::unordered_map<std::string, std::function<void(NavigationDirection)>> customNavigation_;
    float navigationDelay_{0.0f};
    bool navigationWrap_{false};
    bool navigationRepeat_{false};
    float navigationSpeed_{1.0f};

    // Navigation callbacks
    std::function<void(const std::string&, NavigationDirection)> onNavigate_;
    std::function<void(const std::string&, const std::string&)> onFocusChange_;
    std::function<void(const NavigationHistory&)> onHistoryChange_;

    // Helper methods
    std::string findNextComponent(NavigationDirection direction);
    std::string findPreviousComponent(NavigationDirection direction);
    std::string findFirstComponent();
    std::string findLastComponent();
    std::string findParentComponent();
    std::string findChildComponent();
    bool isValidNavigation(const std::string& componentId);
    void updateNavigationState();
    void handleNavigationEvent(const std::string& componentId, NavigationDirection direction);
    void handleFocusEvent(const std::string& oldId, const std::string& newId);
    void handleHistoryEvent(const NavigationHistory& history);
    void validateNavigationState();
    void backupNavigationState();
    void restoreNavigationState();
};

} // namespace ui
} // namespace void_contingency
```

### 2. Create Menu Navigation Manager

Create `include/ui/menu/MenuNavigationManager.hpp`:

```cpp
#pragma once

#include "ui/menu/MenuNavigation.hpp"
#include <memory>
#include <unordered_map>

namespace void_contingency {
namespace ui {

class MenuNavigationManager {
public:
    static MenuNavigationManager& getInstance();

    // Navigation management
    void addNavigation(const std::string& id, std::shared_ptr<MenuNavigation> navigation);
    void removeNavigation(const std::string& id);
    void updateNavigation(const std::string& id, std::shared_ptr<MenuNavigation> navigation);
    std::shared_ptr<MenuNavigation> getNavigation(const std::string& id) const;
    std::vector<std::shared_ptr<MenuNavigation>> getAllNavigations() const;

    // Navigation control
    void handleInput(const InputEvent& event);
    void update();
    void render(Renderer& renderer);

    // Navigation cleanup
    void cleanup();
    void reset();
    void clear();

private:
    MenuNavigationManager() = default;
    ~MenuNavigationManager() = default;
    MenuNavigationManager(const MenuNavigationManager&) = delete;
    MenuNavigationManager& operator=(const MenuNavigationManager&) = delete;

    // Navigation storage
    std::unordered_map<std::string, std::shared_ptr<MenuNavigation>> navigations_;
    std::shared_ptr<MenuNavigation> activeNavigation_;

    // Helper methods
    void updateNavigationState();
    void handleNavigationInput(const InputEvent& event);
    void notifyNavigationCallbacks();
    void cleanupNavigationResources();
    void validateNavigationState();
    void backupNavigationState();
    void restoreNavigationState();
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create Menu Navigation View

Create `include/ui/menu/MenuNavigationView.hpp`:

```cpp
#pragma once

#include "ui/menu/MenuNavigation.hpp"
#include "ui/components/Component.hpp"
#include <memory>

namespace void_contingency {
namespace ui {

class MenuNavigationView : public Component {
public:
    MenuNavigationView();
    ~MenuNavigationView() override;

    // View control
    void setNavigation(std::shared_ptr<MenuNavigation> navigation);
    void setNavigations(const std::vector<std::shared_ptr<MenuNavigation>>& navigations);

    // Interactive features
    void setInteractive(bool interactive) { interactive_ = interactive; }
    bool isInteractive() const { return interactive_; }

    // View customization
    void setShowHistory(bool show) { showHistory_ = show; }
    void setShowFocus(bool show) { showFocus_ = show; }
    void setShowPatterns(bool show) { showPatterns_ = show; }
    void setShowControls(bool show) { showControls_ = show; }

protected:
    void render(Renderer& renderer) override;
    void handleInput(const InputEvent& event) override;
    void update() override;

private:
    // View state
    std::shared_ptr<MenuNavigation> navigation_;
    std::vector<std::shared_ptr<MenuNavigation>> navigations_;
    bool interactive_{false};
    bool showHistory_{true};
    bool showFocus_{true};
    bool showPatterns_{true};
    bool showControls_{true};

    // Helper methods
    void renderHistory(Renderer& renderer);
    void renderFocus(Renderer& renderer);
    void renderPatterns(Renderer& renderer);
    void renderControls(Renderer& renderer);
    void handleNavigationInput(const InputEvent& event);
    void updateVisualization();
};

} // namespace ui
} // namespace void_contingency
```

### 4. Create Menu Navigation Tests

Create `tests/unit/ui/menu/NavigationTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/menu/MenuNavigation.hpp"
#include "ui/menu/MenuNavigationManager.hpp"
#include "ui/menu/MenuNavigationView.hpp"

using namespace void_contingency::ui;

TEST(MenuNavigationTest, NavigationCreation) {
    auto navigation = std::make_shared<MenuNavigation>();
    navigation->setNavigationMode(NavigationMode::Keyboard);

    EXPECT_EQ(navigation->getNavigationMode(), NavigationMode::Keyboard);
}

TEST(MenuNavigationTest, FocusManagement) {
    auto navigation = std::make_shared<MenuNavigation>();
    navigation->setFocus("test_component");

    EXPECT_EQ(navigation->getFocusedComponentId(), "test_component");
    EXPECT_TRUE(navigation->hasFocus("test_component"));
}

TEST(MenuNavigationTest, NavigationHistory) {
    auto navigation = std::make_shared<MenuNavigation>();

    NavigationHistory history;
    history.componentId = "test_component";
    history.mode = NavigationMode::Keyboard;
    history.timestamp = 0.0f;

    navigation->pushHistory(history);
    EXPECT_EQ(navigation->getCurrentHistory().componentId, "test_component");
}

TEST(MenuNavigationManagerTest, NavigationManagement) {
    auto& manager = MenuNavigationManager::getInstance();

    // Create test navigation
    auto navigation = std::make_shared<MenuNavigation>();
    navigation->setNavigationMode(NavigationMode::Keyboard);

    // Add navigation
    manager.addNavigation("test_navigation", navigation);

    // Verify navigation
    auto retrievedNavigation = manager.getNavigation("test_navigation");
    EXPECT_EQ(retrievedNavigation->getNavigationMode(), NavigationMode::Keyboard);
}

TEST(MenuNavigationViewTest, ViewRendering) {
    MenuNavigationView view;

    // Create test navigation
    auto navigation = std::make_shared<MenuNavigation>();
    navigation->setNavigationMode(NavigationMode::Keyboard);

    // Set navigation
    view.setNavigation(navigation);

    // Verify view state
    EXPECT_FALSE(view.isInteractive());
    view.setInteractive(true);
    EXPECT_TRUE(view.isInteractive());
}
```

## Next Steps

1. Implement menu state management
2. Add menu transitions
3. Implement menu input handling
4. Add menu persistence

## Common Pitfalls

1. **Focus Management**: Handle focus changes correctly
2. **Navigation History**: Maintain proper history state
3. **Input Handling**: Process navigation input properly
4. **State Management**: Handle navigation state correctly

## Best Practices

1. Use proper navigation patterns
2. Implement focus management
3. Handle navigation events
4. Manage navigation state
5. Support customization

## Additional Resources

- [Analytics System Tutorial](../analytics/analytics_system.md)
- [Menu State Management Tutorial](./menu_state_management.md)
- [Menu Transitions Tutorial](./menu_transitions.md)
- [Menu Input Handling Tutorial](./menu_input_handling.md)
- [Menu Persistence Tutorial](./menu_persistence.md)
- [Menu Components Tutorial](./menu_components.md)
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
- [HUD Styling Tutorial](./hud_styling.md)
- [HUD Persistence Tutorial](./hud_persistence.md)
- [HUD Debugging Tutorial](./hud_debugging.md)
- [HUD Analytics Tutorial](./hud_analytics.md)
- [HUD Optimization Tutorial](./hud_optimization.md)
- [HUD Sharing Tutorial](./hud_sharing.md)
- [HUD Testing Tutorial](./hud_testing.md)
- [HUD Profiling Tutorial](./hud_profiling.md)
- [HUD Benchmarking Tutorial](./hud_benchmarking.md)
