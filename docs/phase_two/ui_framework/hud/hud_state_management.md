# Implementing HUD State Management

## Navigation

- Previous: [HUD Animations Tutorial](./hud_animations.md)
- Next: [HUD Transitions Tutorial](./hud_transitions.md)

This tutorial will guide you through implementing HUD state management for "Void Contingency", providing robust handling of HUD element states and data synchronization.

## Overview

The HUD state management system will:

- Manage HUD element states
- Handle state transitions
- Support state persistence
- Enable state synchronization
- Provide state history
- Support state validation

## Implementation Steps

### 1. Create HUD State Types

Create `include/ui/hud/HUDState.hpp`:

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

enum class StateType {
    None,
    Visible,
    Hidden,
    Disabled,
    Loading,
    Error,
    Success,
    Warning,
    Custom
};

struct StateTransition {
    StateType fromState;
    StateType toState;
    float duration{0.0f};
    std::function<void()> onStart;
    std::function<void()> onComplete;
    std::function<void(float)> onUpdate;
};

struct StateHistory {
    StateType state;
    std::chrono::system_clock::time_point timestamp;
    std::string reason;
};

struct StateValidation {
    bool isValid{true};
    std::string errorMessage;
    std::vector<std::string> warnings;
};

struct HUDState {
    std::string id;
    StateType type;
    std::vector<StateTransition> transitions;
    std::vector<StateHistory> history;
    StateValidation validation;
    std::unordered_map<std::string, std::string> metadata;
    std::chrono::system_clock::time_point lastUpdated;
};

using StateCallback = std::function<void(const HUDState&)>;

} // namespace ui
} // namespace void_contingency
```

### 2. Create HUD State Manager

Create `include/ui/hud/HUDStateManager.hpp`:

```cpp
#pragma once

#include "ui/hud/HUDState.hpp"
#include <memory>
#include <vector>
#include <unordered_map>

namespace void_contingency {
namespace ui {

class HUDStateManager {
public:
    static HUDStateManager& getInstance();

    // State management
    void addState(const HUDState& state);
    void removeState(const std::string& id);
    void updateState(const std::string& id, const HUDState& state);
    void transitionState(const std::string& id, StateType newState);

    // State control
    void update(float deltaTime);
    void saveStates();
    void loadStates();
    void resetStates();

    // State query
    HUDState getState(const std::string& id) const;
    std::vector<HUDState> getAllStates() const;
    bool hasState(const std::string& id) const;
    StateType getCurrentState(const std::string& id) const;

    // State subscription
    void subscribeToState(const std::string& id, StateCallback callback);
    void unsubscribeFromState(const std::string& id, StateCallback callback);

private:
    HUDStateManager() = default;
    ~HUDStateManager() = default;
    HUDStateManager(const HUDStateManager&) = delete;
    HUDStateManager& operator=(const HUDStateManager&) = delete;

    // State storage
    std::unordered_map<std::string, HUDState> states_;
    std::unordered_map<std::string, std::vector<StateCallback>> callbacks_;

    // Helper methods
    void validateState(const HUDState& state);
    void updateStateTransition(const std::string& id, float deltaTime);
    void notifyCallbacks(const std::string& id, const HUDState& state);
    void addStateHistory(HUDState& state, StateType newState, const std::string& reason);
    bool checkStateTransition(const HUDState& state, StateType newState);
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create HUD State View

Create `include/ui/hud/HUDStateView.hpp`:

```cpp
#pragma once

#include "ui/hud/HUDState.hpp"
#include "ui/components/Component.hpp"
#include <memory>

namespace void_contingency {
namespace ui {

class HUDStateView : public Component {
public:
    HUDStateView();
    ~HUDStateView() override;

    // View control
    void setState(const HUDState& state);
    void setStateTransition(const StateTransition& transition);
    void setStateValidation(const StateValidation& validation);

    // Interactive features
    void setInteractive(bool interactive) { interactive_ = interactive; }
    bool isInteractive() const { return interactive_; }

    // View customization
    void setShowHistory(bool show) { showHistory_ = show; }
    void setShowValidation(bool show) { showValidation_ = show; }
    void setShowMetadata(bool show) { showMetadata_ = show; }

protected:
    void render(Renderer& renderer) override;
    void handleInput(const InputEvent& event) override;
    void update() override;

private:
    // View state
    HUDState currentState_;
    StateTransition currentTransition_;
    StateValidation currentValidation_;
    bool interactive_{false};
    bool showHistory_{true};
    bool showValidation_{true};
    bool showMetadata_{true};

    // Helper methods
    void renderHistory(Renderer& renderer);
    void renderValidation(Renderer& renderer);
    void renderMetadata(Renderer& renderer);
    void handleStateInput(const InputEvent& event);
    void updateVisualization();
};

} // namespace ui
} // namespace void_contingency
```

### 4. Create State Tests

Create `tests/unit/ui/hud/StateTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/hud/HUDState.hpp"
#include "ui/hud/HUDStateManager.hpp"
#include "ui/hud/HUDStateView.hpp"

using namespace void_contingency::ui;

TEST(HUDStateManagerTest, StateManagement) {
    auto& manager = HUDStateManager::getInstance();

    // Create test state
    HUDState state;
    state.id = "test_state";
    state.type = StateType::Visible;
    state.validation.isValid = true;

    // Add state
    manager.addState(state);

    // Verify state
    EXPECT_TRUE(manager.hasState("test_state"));
    EXPECT_EQ(manager.getCurrentState("test_state"), StateType::Visible);

    // Transition state
    manager.transitionState("test_state", StateType::Hidden);
    EXPECT_EQ(manager.getCurrentState("test_state"), StateType::Hidden);

    // Update state
    state.type = StateType::Disabled;
    manager.updateState("test_state", state);
    EXPECT_EQ(manager.getCurrentState("test_state"), StateType::Disabled);
}

TEST(HUDStateViewTest, ViewRendering) {
    HUDStateView view;

    // Set test state
    HUDState state;
    state.id = "test_state";
    state.type = StateType::Visible;
    view.setState(state);

    // Set test transition
    StateTransition transition;
    transition.fromState = StateType::Visible;
    transition.toState = StateType::Hidden;
    view.setStateTransition(transition);

    // Verify view state
    EXPECT_FALSE(view.isInteractive());
    view.setInteractive(true);
    EXPECT_TRUE(view.isInteractive());
}
```

## Next Steps

1. Add HUD transitions
2. Implement HUD input handling
3. Add HUD styling
4. Implement HUD persistence

## Common Pitfalls

1. **State Synchronization**: Handle state updates across components
2. **State Validation**: Validate state transitions and data
3. **Performance**: Optimize state updates for many elements
4. **Resource Usage**: Control memory usage for state history

## Best Practices

1. Use efficient state updates
2. Implement proper validation
3. Support state persistence
4. Enable state history
5. Provide clear feedback

## Additional Resources

- [HUD Animations Tutorial](./hud_animations.md)
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
