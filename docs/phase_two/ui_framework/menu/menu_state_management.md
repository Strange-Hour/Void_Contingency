# Implementing Menu State Management

## Navigation

- Previous: [Menu Navigation Tutorial](./menu_navigation.md)
- Next: [Menu Transitions Tutorial](./menu_transitions.md)

This tutorial will guide you through implementing menu state management for "Void Contingency", providing a robust system for managing menu states, transitions, and persistence.

## Overview

The menu state management system will:

- Track menu states
- Handle state transitions
- Manage state persistence
- Support state validation
- Enable state debugging
- Provide state analytics

## Implementation Steps

### 1. Create Menu State Types

Create `include/ui/menu/MenuState.hpp`:

```cpp
#pragma once

#include "ui/menu/MenuComponents.hpp"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include <variant>

namespace void_contingency {
namespace ui {

enum class MenuStateType {
    Initial,
    Active,
    Paused,
    Hidden,
    Disabled,
    Loading,
    Error,
    Success,
    Warning,
    Custom
};

struct MenuStateData {
    std::string id;
    MenuStateType type;
    std::unordered_map<std::string, std::variant<bool, int, float, std::string>> properties;
    std::vector<std::string> tags;
    float timestamp;
    std::string parentId;
    std::vector<std::string> childIds;
};

class MenuState {
public:
    MenuState();
    ~MenuState();

    // State identification
    void setId(const std::string& id) { data_.id = id; }
    const std::string& getId() const { return data_.id; }

    void setType(MenuStateType type) { data_.type = type; }
    MenuStateType getType() const { return data_.type; }

    // State properties
    template<typename T>
    void setProperty(const std::string& key, const T& value) {
        data_.properties[key] = value;
    }

    template<typename T>
    T getProperty(const std::string& key, const T& defaultValue = T()) const {
        auto it = data_.properties.find(key);
        if (it != data_.properties.end()) {
            return std::get<T>(it->second);
        }
        return defaultValue;
    }

    // State hierarchy
    void setParentId(const std::string& parentId) { data_.parentId = parentId; }
    const std::string& getParentId() const { return data_.parentId; }

    void addChildId(const std::string& childId) { data_.childIds.push_back(childId); }
    void removeChildId(const std::string& childId);
    const std::vector<std::string>& getChildIds() const { return data_.childIds; }

    // State tags
    void addTag(const std::string& tag) { data_.tags.push_back(tag); }
    void removeTag(const std::string& tag);
    bool hasTag(const std::string& tag) const;
    const std::vector<std::string>& getTags() const { return data_.tags; }

    // State timing
    void setTimestamp(float timestamp) { data_.timestamp = timestamp; }
    float getTimestamp() const { return data_.timestamp; }

    // State validation
    bool isValid() const;
    std::vector<std::string> validate() const;

    // State comparison
    bool equals(const MenuState& other) const;
    bool matches(const std::string& pattern) const;

    // State serialization
    std::string serialize() const;
    void deserialize(const std::string& data);

private:
    MenuStateData data_;

    // Helper methods
    void validateProperties();
    void validateHierarchy();
    void validateTags();
    void validateTiming();
    void backupState();
    void restoreState();
};

} // namespace ui
} // namespace void_contingency
```

### 2. Create Menu State Manager

Create `include/ui/menu/MenuStateManager.hpp`:

```cpp
#pragma once

#include "ui/menu/MenuState.hpp"
#include <memory>
#include <vector>
#include <unordered_map>
#include <stack>
#include <functional>

namespace void_contingency {
namespace ui {

class MenuStateManager {
public:
    static MenuStateManager& getInstance();

    // State management
    void addState(std::shared_ptr<MenuState> state);
    void removeState(const std::string& id);
    void updateState(const std::string& id, std::shared_ptr<MenuState> state);
    std::shared_ptr<MenuState> getState(const std::string& id) const;
    std::vector<std::shared_ptr<MenuState>> getAllStates() const;

    // State transitions
    void transitionTo(const std::string& targetId);
    void transitionBack();
    void transitionForward();
    void transitionToInitial();
    void transitionToError(const std::string& message);

    // State history
    void pushHistory(const std::string& stateId);
    void popHistory();
    void clearHistory();
    const std::string& getCurrentStateId() const;
    const std::stack<std::string>& getHistory() const;

    // State events
    void setOnStateChange(std::function<void(const std::string&, const std::string&)> callback);
    void setOnStateEnter(std::function<void(const std::string&)> callback);
    void setOnStateExit(std::function<void(const std::string&)> callback);
    void setOnStateError(std::function<void(const std::string&, const std::string&)> callback);

    // State validation
    bool validateState(const std::string& id) const;
    std::vector<std::string> validateAllStates() const;

    // State persistence
    void saveStates(const std::string& filename);
    void loadStates(const std::string& filename);
    void exportStates(const std::string& filename);
    void importStates(const std::string& filename);

    // State debugging
    void enableDebugging(bool enable);
    void setDebugLevel(int level);
    void logStateChange(const std::string& message);

    // State analytics
    void trackStateUsage(const std::string& stateId);
    void trackStateDuration(const std::string& stateId, float duration);
    void generateStateReport(const std::string& filename);

    // State cleanup
    void cleanup();
    void reset();
    void clear();

private:
    MenuStateManager() = default;
    ~MenuStateManager() = default;
    MenuStateManager(const MenuStateManager&) = delete;
    MenuStateManager& operator=(const MenuStateManager&) = delete;

    // State storage
    std::unordered_map<std::string, std::shared_ptr<MenuState>> states_;
    std::stack<std::string> history_;
    std::string currentStateId_;

    // State callbacks
    std::function<void(const std::string&, const std::string&)> onStateChange_;
    std::function<void(const std::string&)> onStateEnter_;
    std::function<void(const std::string&)> onStateExit_;
    std::function<void(const std::string&, const std::string&)> onStateError_;

    // State tracking
    std::unordered_map<std::string, int> stateUsage_;
    std::unordered_map<std::string, float> stateDuration_;
    bool debuggingEnabled_{false};
    int debugLevel_{0};

    // Helper methods
    void updateStateHistory();
    void handleStateTransition(const std::string& fromId, const std::string& toId);
    void validateStateTransition(const std::string& fromId, const std::string& toId);
    void notifyStateCallbacks();
    void cleanupStateResources();
    void validateStateHierarchy();
    void backupStateHistory();
    void restoreStateHistory();
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create Menu State View

Create `include/ui/menu/MenuStateView.hpp`:

```cpp
#pragma once

#include "ui/menu/MenuState.hpp"
#include "ui/components/Component.hpp"
#include <memory>

namespace void_contingency {
namespace ui {

class MenuStateView : public Component {
public:
    MenuStateView();
    ~MenuStateView() override;

    // View control
    void setState(std::shared_ptr<MenuState> state);
    void setStates(const std::vector<std::shared_ptr<MenuState>>& states);

    // Interactive features
    void setInteractive(bool interactive) { interactive_ = interactive; }
    bool isInteractive() const { return interactive_; }

    // View customization
    void setShowProperties(bool show) { showProperties_ = show; }
    void setShowHierarchy(bool show) { showHierarchy_ = show; }
    void setShowHistory(bool show) { showHistory_ = show; }
    void setShowAnalytics(bool show) { showAnalytics_ = show; }

protected:
    void render(Renderer& renderer) override;
    void handleInput(const InputEvent& event) override;
    void update() override;

private:
    // View state
    std::shared_ptr<MenuState> state_;
    std::vector<std::shared_ptr<MenuState>> states_;
    bool interactive_{false};
    bool showProperties_{true};
    bool showHierarchy_{true};
    bool showHistory_{true};
    bool showAnalytics_{true};

    // Helper methods
    void renderProperties(Renderer& renderer);
    void renderHierarchy(Renderer& renderer);
    void renderHistory(Renderer& renderer);
    void renderAnalytics(Renderer& renderer);
    void handleStateInput(const InputEvent& event);
    void updateVisualization();
};

} // namespace ui
} // namespace void_contingency
```

### 4. Create Menu State Tests

Create `tests/unit/ui/menu/StateTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/menu/MenuState.hpp"
#include "ui/menu/MenuStateManager.hpp"
#include "ui/menu/MenuStateView.hpp"

using namespace void_contingency::ui;

TEST(MenuStateTest, StateCreation) {
    auto state = std::make_shared<MenuState>();
    state->setId("test_state");
    state->setType(MenuStateType::Active);

    EXPECT_EQ(state->getId(), "test_state");
    EXPECT_EQ(state->getType(), MenuStateType::Active);
}

TEST(MenuStateTest, StateProperties) {
    auto state = std::make_shared<MenuState>();
    state->setId("test_state");

    state->setProperty("visible", true);
    state->setProperty("enabled", true);
    state->setProperty("value", 42);

    EXPECT_TRUE(state->getProperty<bool>("visible"));
    EXPECT_TRUE(state->getProperty<bool>("enabled"));
    EXPECT_EQ(state->getProperty<int>("value"), 42);
}

TEST(MenuStateTest, StateHierarchy) {
    auto parent = std::make_shared<MenuState>();
    parent->setId("parent");
    auto child = std::make_shared<MenuState>();
    child->setId("child");

    child->setParentId("parent");
    parent->addChildId("child");

    EXPECT_EQ(child->getParentId(), "parent");
    EXPECT_EQ(parent->getChildIds().size(), 1);
    EXPECT_EQ(parent->getChildIds()[0], "child");
}

TEST(MenuStateManagerTest, StateManagement) {
    auto& manager = MenuStateManager::getInstance();

    // Create test state
    auto state = std::make_shared<MenuState>();
    state->setId("test_state");
    state->setType(MenuStateType::Active);

    // Add state
    manager.addState(state);

    // Verify state
    auto retrievedState = manager.getState("test_state");
    EXPECT_EQ(retrievedState->getId(), "test_state");
    EXPECT_EQ(retrievedState->getType(), MenuStateType::Active);
}

TEST(MenuStateViewTest, ViewRendering) {
    MenuStateView view;

    // Create test state
    auto state = std::make_shared<MenuState>();
    state->setId("test_state");
    state->setType(MenuStateType::Active);

    // Set state
    view.setState(state);

    // Verify view state
    EXPECT_FALSE(view.isInteractive());
    view.setInteractive(true);
    EXPECT_TRUE(view.isInteractive());
}
```

## Next Steps

1. Implement menu transitions
2. Add menu input handling
3. Implement menu persistence
4. Add menu debugging

## Common Pitfalls

1. **State Transitions**: Handle state changes correctly
2. **State Validation**: Validate state properties properly
3. **State History**: Maintain proper history state
4. **State Persistence**: Handle state saving/loading correctly

## Best Practices

1. Use proper state management
2. Implement state validation
3. Handle state transitions
4. Manage state history
5. Support state persistence

## Additional Resources

- [Menu Components Tutorial](./menu_components.md)
- [Menu Navigation Tutorial](./menu_navigation.md)
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
- [Menu Transitions Tutorial](./menu_transitions.md)
- [Menu Input Handling Tutorial](./menu_input_handling.md)
- [Menu Persistence Tutorial](./menu_persistence.md)
- [Menu Debugging Tutorial](./menu_debugging.md)
