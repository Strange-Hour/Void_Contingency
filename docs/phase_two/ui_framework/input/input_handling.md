# Implementing Input Handling

## Navigation

- Previous: [HUD Benchmarking Tutorial](../hud/hud_benchmarking.md)
- Next: [Input Gestures Tutorial](./input_gestures.md)

This tutorial will guide you through implementing the input handling system for the UI framework in "Void Contingency", focusing on event processing, state management, and input validation.

## Overview

The input handling system will:

- Process input events (mouse, keyboard, touch)
- Manage input state
- Handle input mapping
- Validate input
- Support input focus
- Enable input blocking

## Implementation Steps

### 1. Create Input Event System

Create `include/ui/input/InputEvent.hpp`:

```cpp
#pragma once

#include <glm/glm.hpp>
#include <string>
#include <variant>

namespace void_contingency {
namespace ui {

enum class InputEventType {
    MouseMove,
    MouseButton,
    MouseWheel,
    Key,
    Touch,
    TextInput
};

enum class MouseButton {
    Left,
    Right,
    Middle,
    Back,
    Forward
};

enum class KeyModifier {
    None = 0,
    Shift = 1 << 0,
    Control = 1 << 1,
    Alt = 1 << 2,
    Super = 1 << 3
};

struct MouseEvent {
    glm::vec2 position;
    glm::vec2 delta;
    MouseButton button;
    bool pressed;
    uint32_t modifiers;
};

struct KeyEvent {
    uint32_t key;
    bool pressed;
    bool repeat;
    uint32_t modifiers;
};

struct TouchEvent {
    uint32_t id;
    glm::vec2 position;
    glm::vec2 delta;
    bool pressed;
    float pressure;
};

struct TextEvent {
    std::string text;
    uint32_t modifiers;
};

using InputEventData = std::variant<
    MouseEvent,
    KeyEvent,
    TouchEvent,
    TextEvent
>;

class InputEvent {
public:
    InputEvent(InputEventType type, InputEventData data)
        : type_(type), data_(std::move(data)) {}

    InputEventType getType() const { return type_; }

    template<typename T>
    const T& getData() const {
        return std::get<T>(data_);
    }

private:
    InputEventType type_;
    InputEventData data_;
};

} // namespace ui
} // namespace void_contingency
```

### 2. Create Input State Manager

Create `include/ui/input/InputState.hpp`:

```cpp
#pragma once

#include "ui/input/InputEvent.hpp"
#include <unordered_map>
#include <bitset>
#include <glm/glm.hpp>

namespace void_contingency {
namespace ui {

class InputState {
public:
    static InputState& getInstance();

    // State management
    void update();
    void reset();

    // Mouse state
    glm::vec2 getMousePosition() const { return mousePosition_; }
    glm::vec2 getMouseDelta() const { return mouseDelta_; }
    bool isMouseButtonPressed(MouseButton button) const;
    bool isMouseButtonJustPressed(MouseButton button) const;
    bool isMouseButtonJustReleased(MouseButton button) const;

    // Keyboard state
    bool isKeyPressed(uint32_t key) const;
    bool isKeyJustPressed(uint32_t key) const;
    bool isKeyJustReleased(uint32_t key) const;
    uint32_t getKeyModifiers() const { return keyModifiers_; }

    // Touch state
    bool isTouchActive(uint32_t id) const;
    glm::vec2 getTouchPosition(uint32_t id) const;
    float getTouchPressure(uint32_t id) const;

    // Focus management
    void setFocus(Component* component);
    Component* getFocus() const { return focus_; }
    bool hasFocus(Component* component) const;

    // Input blocking
    void setBlocked(bool blocked) { blocked_ = blocked; }
    bool isBlocked() const { return blocked_; }

private:
    InputState() = default;
    ~InputState() = default;
    InputState(const InputState&) = delete;
    InputState& operator=(const InputState&) = delete;

    // Mouse state
    glm::vec2 mousePosition_;
    glm::vec2 mouseDelta_;
    std::bitset<5> mouseButtons_;
    std::bitset<5> mouseButtonsPrev_;

    // Keyboard state
    std::unordered_map<uint32_t, bool> keys_;
    std::unordered_map<uint32_t, bool> keysPrev_;
    uint32_t keyModifiers_{0};

    // Touch state
    struct TouchState {
        glm::vec2 position;
        float pressure;
        bool active;
    };
    std::unordered_map<uint32_t, TouchState> touches_;

    // Focus state
    Component* focus_{nullptr};
    bool blocked_{false};

    // Helper methods
    void updateMouseState();
    void updateKeyboardState();
    void updateTouchState();
    void updateFocus();
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create Input Handler

Create `include/ui/input/InputHandler.hpp`:

```cpp
#pragma once

#include "ui/input/InputEvent.hpp"
#include "ui/input/InputState.hpp"
#include <queue>
#include <functional>

namespace void_contingency {
namespace ui {

using InputCallback = std::function<void(const InputEvent&)>;

class InputHandler {
public:
    static InputHandler& getInstance();

    // Event processing
    void processEvent(const InputEvent& event);
    void update();

    // Event subscription
    void subscribe(InputEventType type, InputCallback callback);
    void unsubscribe(InputEventType type, InputCallback callback);

    // Input mapping
    void addMapping(const std::string& name, const std::vector<InputEvent>& events);
    void removeMapping(const std::string& name);
    bool isMappingActive(const std::string& name) const;

    // Input validation
    bool validateEvent(const InputEvent& event) const;
    void setValidationRules(const std::vector<std::function<bool(const InputEvent&)>>& rules);

private:
    InputHandler() = default;
    ~InputHandler() = default;
    InputHandler(const InputHandler&) = delete;
    InputHandler& operator=(const InputHandler&) = delete;

    // Event queue
    std::queue<InputEvent> eventQueue_;

    // Event handlers
    std::unordered_map<InputEventType, std::vector<InputCallback>> handlers_;

    // Input mappings
    struct InputMapping {
        std::string name;
        std::vector<InputEvent> events;
        bool active;
    };
    std::unordered_map<std::string, InputMapping> mappings_;

    // Validation rules
    std::vector<std::function<bool(const InputEvent&)>> validationRules_;

    // Helper methods
    void processEventQueue();
    void updateMappings();
    bool checkMapping(const InputMapping& mapping) const;
};

} // namespace ui
} // namespace void_contingency
```

### 4. Create Input Tests

Create `tests/unit/ui/input/InputTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/input/InputEvent.hpp"
#include "ui/input/InputState.hpp"
#include "ui/input/InputHandler.hpp"
#include "ui/components/Component.hpp"

using namespace void_contingency::ui;

TEST(InputEventTest, EventCreation) {
    // Create mouse event
    MouseEvent mouseEvent{
        glm::vec2(100.0f, 200.0f),
        glm::vec2(10.0f, 20.0f),
        MouseButton::Left,
        true,
        0
    };
    InputEvent event(InputEventType::MouseButton, mouseEvent);

    EXPECT_EQ(event.getType(), InputEventType::MouseButton);
    const auto& data = event.getData<MouseEvent>();
    EXPECT_EQ(data.position, glm::vec2(100.0f, 200.0f));
    EXPECT_EQ(data.button, MouseButton::Left);
    EXPECT_TRUE(data.pressed);
}

TEST(InputStateTest, StateManagement) {
    auto& state = InputState::getInstance();

    // Test mouse state
    state.update();
    EXPECT_FALSE(state.isMouseButtonPressed(MouseButton::Left));

    // Test keyboard state
    EXPECT_FALSE(state.isKeyPressed(0));
    EXPECT_EQ(state.getKeyModifiers(), 0);

    // Test focus
    Component component;
    state.setFocus(&component);
    EXPECT_EQ(state.getFocus(), &component);
    EXPECT_TRUE(state.hasFocus(&component));
}

TEST(InputHandlerTest, EventProcessing) {
    auto& handler = InputHandler::getInstance();

    // Test event subscription
    bool eventReceived = false;
    handler.subscribe(InputEventType::MouseButton, [&](const InputEvent&) {
        eventReceived = true;
    });

    // Create and process event
    MouseEvent mouseEvent{
        glm::vec2(0.0f),
        glm::vec2(0.0f),
        MouseButton::Left,
        true,
        0
    };
    handler.processEvent(InputEvent(InputEventType::MouseButton, mouseEvent));
    handler.update();

    EXPECT_TRUE(eventReceived);
}

TEST(InputHandlerTest, InputMapping) {
    auto& handler = InputHandler::getInstance();

    // Create input mapping
    std::vector<InputEvent> events;
    events.emplace_back(InputEventType::Key, KeyEvent{0, true, false, 0});
    handler.addMapping("test_action", events);

    // Test mapping
    EXPECT_TRUE(handler.isMappingActive("test_action"));

    // Remove mapping
    handler.removeMapping("test_action");
    EXPECT_FALSE(handler.isMappingActive("test_action"));
}
```

## Next Steps

1. Add input gesture recognition
2. Implement input recording and playback
3. Add input configuration system
4. Implement input debugging tools

## Common Pitfalls

1. **Event Ordering**: Handle event order correctly
2. **State Management**: Maintain consistent state
3. **Focus Handling**: Manage focus properly
4. **Event Propagation**: Control event flow

## Best Practices

1. Use event queue for processing
2. Implement proper focus management
3. Support input mapping
4. Validate input events
5. Handle input blocking

## Additional Resources

- [HUD Benchmarking Tutorial](../hud/hud_benchmarking.md)
- [Input Gestures Tutorial](./input_gestures.md)
- [Gesture Recording Tutorial](./gesture_recording.md)
- [Gesture Analysis Tutorial](./gesture_analysis.md)
- [Gesture Visualization Tutorial](./gesture_visualization.md)
- [Gesture Templates Tutorial](./gesture_templates.md)
- [Gesture Training Tutorial](./gesture_training.md)
- [Gesture Export/Import Tutorial](./gesture_export_import.md)
- [Gesture Collaboration Tutorial](./gesture_collaboration.md)
- [Gesture Recognition Tutorial](./gesture_recognition.md)
- [Gesture Customization Tutorial](./gesture_customization.md)
- [Gesture Validation Tutorial](./gesture_validation.md)
- [Gesture Analytics Tutorial](./gesture_analytics.md)
- [Gesture Optimization Tutorial](./gesture_optimization.md)
- [Gesture Sharing Tutorial](./gesture_sharing.md)
