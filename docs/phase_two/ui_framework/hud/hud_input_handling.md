# Implementing HUD Input Handling

## Navigation

- Previous: [HUD Transitions Tutorial](./hud_transitions.md)
- Next: [HUD Styling Tutorial](./hud_styling.md)

This tutorial will guide you through implementing an input handling system for the HUD in "Void Contingency", focusing on event processing, input mapping, and feedback.

## Overview

The HUD input handling system will:

- Process input events for HUD elements
- Support input mapping and configuration
- Provide input feedback
- Handle input state management
- Enable input customization
- Support input debugging

## Implementation Steps

### 1. Create HUD Input Types

Create `include/ui/hud/HUDInput.hpp`:

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

enum class InputAction {
    None,
    Select,
    Deselect,
    Toggle,
    Show,
    Hide,
    Move,
    Resize,
    Custom
};

enum class InputState {
    None,
    Pressed,
    Released,
    Held,
    DoublePressed,
    LongPressed
};

struct InputMapping {
    std::string id;
    InputAction action;
    std::vector<std::string> keys;
    std::vector<std::string> modifiers;
    float threshold{0.0f};
    float repeatDelay{0.0f};
    bool enabled{true};
};

struct InputFeedback {
    bool visual{true};
    bool audio{true};
    bool haptic{true};
    float intensity{1.0f};
    std::string soundId;
    std::string effectId;
};

struct InputState {
    bool isActive{false};
    bool isBlocked{false};
    float pressure{0.0f};
    std::chrono::steady_clock::time_point lastPressed;
    std::chrono::steady_clock::time_point lastReleased;
    std::vector<std::string> activeModifiers;
};

struct HUDInput {
    std::string id;
    InputMapping mapping;
    InputFeedback feedback;
    InputState state;
    std::function<void(const InputEvent&)> eventCallback;
    std::function<void(const InputState&)> stateCallback;
};

using InputCallback = std::function<void(const HUDInput&)>;

} // namespace ui
} // namespace void_contingency
```

### 2. Create HUD Input Manager

Create `include/ui/hud/HUDInputManager.hpp`:

```cpp
#pragma once

#include "ui/hud/HUDInput.hpp"
#include <memory>
#include <vector>
#include <unordered_map>

namespace void_contingency {
namespace ui {

class HUDInputManager {
public:
    static HUDInputManager& getInstance();

    // Input management
    void addInput(const HUDInput& input);
    void removeInput(const std::string& id);
    void updateInput(const std::string& id, const HUDInput& input);
    void enableInput(const std::string& id);
    void disableInput(const std::string& id);

    // Input control
    void update(float deltaTime);
    void processEvent(const InputEvent& event);
    void resetInputs();

    // Input state
    bool isInputActive(const std::string& id) const;
    InputState getInputState(const std::string& id) const;
    std::vector<HUDInput> getActiveInputs() const;

    // Input subscription
    void subscribeToInput(const std::string& id, InputCallback callback);
    void unsubscribeFromInput(const std::string& id, InputCallback callback);

private:
    HUDInputManager() = default;
    ~HUDInputManager() = default;
    HUDInputManager(const HUDInputManager&) = delete;
    HUDInputManager& operator=(const HUDInputManager&) = delete;

    // Input state
    std::unordered_map<std::string, HUDInput> inputs_;
    std::unordered_map<std::string, std::vector<InputCallback>> callbacks_;

    // Helper methods
    void updateInputState(HUDInput& input, float deltaTime);
    void processInputEvent(HUDInput& input, const InputEvent& event);
    void notifyCallbacks(const std::string& id, const HUDInput& input);
    void applyInputFeedback(const HUDInput& input);
    bool checkInputMapping(const HUDInput& input, const InputEvent& event);
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create HUD Input View

Create `include/ui/hud/HUDInputView.hpp`:

```cpp
#pragma once

#include "ui/hud/HUDInput.hpp"
#include "ui/components/Component.hpp"
#include <memory>

namespace void_contingency {
namespace ui {

class HUDInputView : public Component {
public:
    HUDInputView();
    ~HUDInputView() override;

    // View control
    void setInput(const HUDInput& input);
    void setInputMapping(const InputMapping& mapping);
    void setInputState(const InputState& state);

    // Interactive features
    void setInteractive(bool interactive) { interactive_ = interactive; }
    bool isInteractive() const { return interactive_; }

    // View customization
    void setShowMapping(bool show) { showMapping_ = show; }
    void setShowState(bool show) { showState_ = show; }
    void setShowFeedback(bool show) { showFeedback_ = show; }

protected:
    void render(Renderer& renderer) override;
    void handleInput(const InputEvent& event) override;
    void update() override;

private:
    // View state
    HUDInput currentInput_;
    InputMapping currentMapping_;
    InputState currentState_;
    bool interactive_{false};
    bool showMapping_{true};
    bool showState_{true};
    bool showFeedback_{true};

    // Helper methods
    void renderMapping(Renderer& renderer);
    void renderState(Renderer& renderer);
    void renderFeedback(Renderer& renderer);
    void handleInputEvent(const InputEvent& event);
    void updateVisualization();
};

} // namespace ui
} // namespace void_contingency
```

### 4. Create Input Tests

Create `tests/unit/ui/hud/InputTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/hud/HUDInput.hpp"
#include "ui/hud/HUDInputManager.hpp"
#include "ui/hud/HUDInputView.hpp"

using namespace void_contingency::ui;

TEST(HUDInputManagerTest, InputManagement) {
    auto& manager = HUDInputManager::getInstance();

    // Create test input
    HUDInput input;
    input.id = "test_input";
    input.mapping.action = InputAction::Select;
    input.mapping.keys = {"Space"};
    input.eventCallback = [](const InputEvent& event) {
        // Test callback
    };

    // Add input
    manager.addInput(input);

    // Verify input
    EXPECT_TRUE(manager.isInputActive("test_input"));
    EXPECT_EQ(manager.getInputState("test_input").pressure, 0.0f);

    // Process event
    InputEvent event;
    event.type = InputEventType::KeyPress;
    event.key = "Space";
    manager.processEvent(event);
    EXPECT_GT(manager.getInputState("test_input").pressure, 0.0f);

    // Reset input
    manager.resetInputs();
    EXPECT_EQ(manager.getInputState("test_input").pressure, 0.0f);
}

TEST(HUDInputViewTest, ViewRendering) {
    HUDInputView view;

    // Set test input
    HUDInput input;
    input.id = "test_input";
    input.mapping.action = InputAction::Select;
    view.setInput(input);

    // Set test state
    InputState state;
    state.isActive = true;
    state.pressure = 1.0f;
    view.setInputState(state);

    // Verify view state
    EXPECT_FALSE(view.isInteractive());
    view.setInteractive(true);
    EXPECT_TRUE(view.isInteractive());
}
```

## Next Steps

1. Add HUD styling
2. Implement HUD persistence
3. Add HUD debugging tools
4. Implement HUD analytics

## Common Pitfalls

1. **Event Processing**: Handle input event propagation
2. **State Management**: Manage input state transitions
3. **Performance**: Optimize input processing for many elements
4. **Resource Usage**: Control memory usage for input mappings

## Best Practices

1. Use efficient input processing
2. Implement proper event handling
3. Support input customization
4. Enable input feedback
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
- [HUD Styling Tutorial](./hud_styling.md)
- [HUD Persistence Tutorial](./hud_persistence.md)
- [HUD Debugging Tutorial](./hud_debugging.md)
- [HUD Analytics Tutorial](./hud_analytics.md)
- [HUD Optimization Tutorial](./hud_optimization.md)
- [HUD Sharing Tutorial](./hud_sharing.md)
- [HUD Testing Tutorial](./hud_testing.md)
- [HUD Profiling Tutorial](./hud_profiling.md)
- [HUD Benchmarking Tutorial](./hud_benchmarking.md)
