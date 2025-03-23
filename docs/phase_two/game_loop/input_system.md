# Implementing the Input System

## Navigation

- Previous: [Game Loop Implementation](./game_loop_implementation.md)
- Next: [State Management Tutorial](./state_management.md)

## What We're Building

We're implementing a robust input system that handles all user interactions with the game. This system processes keyboard, mouse, and gamepad inputs, maps them to game actions, and provides a clean interface for the rest of the game to respond to player input.

## Game Development Concepts Covered

1. **Input Processing**: Handling various input devices and events
2. **Event Systems**: Managing and dispatching input events
3. **Input Mapping**: Converting raw input to meaningful game actions
4. **State Management**: Tracking input states and changes
5. **Input Buffering**: Handling input queues and timing
6. **Device Management**: Supporting multiple input devices
7. **Focus Management**: Handling window focus and input priority
8. **Input Configuration**: Allowing customizable input bindings

## Why This Implementation?

1. **Flexibility**: Support for multiple input devices and configurations
2. **Responsiveness**: Efficient input processing and minimal latency
3. **Extensibility**: Easy to add new input types and mappings
4. **Reliability**: Robust handling of input edge cases
5. **Configurability**: User-customizable input bindings

## Implementation Steps

### 1. Create Input Event Types

Create `include/core/input/InputEvent.hpp`:

```cpp
#pragma once

#include <string>
#include <variant>

namespace void_contingency {
namespace core {

enum class InputType {
    Keyboard,
    Mouse,
    Gamepad
};

enum class InputAction {
    Press,
    Release,
    Repeat,
    Move,
    Axis
};

struct KeyboardEvent {
    int key;
    int scancode;
    InputAction action;
    int mods;
};

struct MouseEvent {
    double xpos;
    double ypos;
    InputAction action;
    int button;
    int mods;
};

struct GamepadEvent {
    int gamepad;
    int button;
    InputAction action;
    float value;
};

using InputEventData = std::variant<KeyboardEvent, MouseEvent, GamepadEvent>;

struct InputEvent {
    InputType type;
    InputAction action;
    InputEventData data;
    float timestamp;
};

} // namespace core
} // namespace void_contingency
```

### 2. Create Input Manager

Create `include/core/input/InputManager.hpp`:

```cpp
#pragma once

#include "core/input/InputEvent.hpp"
#include <queue>
#include <unordered_map>
#include <functional>
#include <string>
#include <memory>

namespace void_contingency {
namespace core {

class InputManager {
public:
    static InputManager& getInstance();

    void initialize();
    void shutdown();

    // Input processing
    void processInput();
    void handleEvent(const InputEvent& event);
    void clearInputBuffer();

    // Input mapping
    void registerAction(const std::string& name, const std::string& key);
    void unregisterAction(const std::string& name);
    bool isActionPressed(const std::string& name) const;
    bool isActionJustPressed(const std::string& name) const;
    bool isActionJustReleased(const std::string& name) const;
    float getActionValue(const std::string& name) const;

    // Input state
    bool isKeyPressed(int key) const;
    bool isMouseButtonPressed(int button) const;
    bool isGamepadButtonPressed(int gamepad, int button) const;
    Vector2f getMousePosition() const;
    Vector2f getMouseDelta() const;
    float getGamepadAxis(int gamepad, int axis) const;

    // Input focus
    void setInputFocus(bool focused);
    bool hasInputFocus() const;

    // Event callbacks
    using InputCallback = std::function<void(const InputEvent&)>;
    void addInputCallback(InputCallback callback);
    void removeInputCallback(InputCallback callback);

private:
    InputManager() = default;
    ~InputManager() = default;
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    void updateInputState();
    void processInputQueue();
    void handleKeyboardEvent(const KeyboardEvent& event);
    void handleMouseEvent(const MouseEvent& event);
    void handleGamepadEvent(const GamepadEvent& event);

    struct InputState {
        bool pressed{false};
        bool justPressed{false};
        bool justReleased{false};
        float value{0.0f};
        float lastValue{0.0f};
    };

    std::queue<InputEvent> inputQueue_;
    std::unordered_map<std::string, InputState> actionStates_;
    std::unordered_map<int, InputState> keyStates_;
    std::unordered_map<int, InputState> mouseButtonStates_;
    std::unordered_map<int, std::unordered_map<int, InputState>> gamepadButtonStates_;

    Vector2f mousePosition_{0.0f, 0.0f};
    Vector2f mouseDelta_{0.0f, 0.0f};
    std::unordered_map<int, std::unordered_map<int, float>> gamepadAxes_;

    std::vector<InputCallback> inputCallbacks_;
    bool inputFocus_{true};
};

} // namespace core
} // namespace void_contingency
```

### 3. Implement Input Manager

Create `src/core/input/InputManager.cpp`:

```cpp
#include "core/input/InputManager.hpp"
#include "core/Logger.hpp"
#include <algorithm>

namespace void_contingency {
namespace core {

InputManager& InputManager::getInstance() {
    static InputManager instance;
    return instance;
}

void InputManager::initialize() {
    Logger::info("Initializing input manager...");
}

void InputManager::shutdown() {
    Logger::info("Shutting down input manager...");
    clearInputBuffer();
}

void InputManager::processInput() {
    if (!inputFocus_) return;

    updateInputState();
    processInputQueue();
}

void InputManager::handleEvent(const InputEvent& event) {
    if (!inputFocus_) return;

    inputQueue_.push(event);
}

void InputManager::clearInputBuffer() {
    while (!inputQueue_.empty()) {
        inputQueue_.pop();
    }
}

void InputManager::registerAction(const std::string& name, const std::string& key) {
    // Convert key string to input event
    InputEvent event;
    // ... key string parsing logic ...

    actionStates_[name] = InputState{};
    Logger::info("Registered action: {} -> {}", name, key);
}

void InputManager::unregisterAction(const std::string& name) {
    actionStates_.erase(name);
    Logger::info("Unregistered action: {}", name);
}

bool InputManager::isActionPressed(const std::string& name) const {
    auto it = actionStates_.find(name);
    return it != actionStates_.end() && it->second.pressed;
}

bool InputManager::isActionJustPressed(const std::string& name) const {
    auto it = actionStates_.find(name);
    return it != actionStates_.end() && it->second.justPressed;
}

bool InputManager::isActionJustReleased(const std::string& name) const {
    auto it = actionStates_.find(name);
    return it != actionStates_.end() && it->second.justReleased;
}

float InputManager::getActionValue(const std::string& name) const {
    auto it = actionStates_.find(name);
    return it != actionStates_.end() ? it->second.value : 0.0f;
}

void InputManager::updateInputState() {
    // Reset just pressed/released states
    for (auto& [name, state] : actionStates_) {
        state.justPressed = false;
        state.justReleased = false;
    }

    for (auto& [key, state] : keyStates_) {
        state.justPressed = false;
        state.justReleased = false;
    }

    for (auto& [button, state] : mouseButtonStates_) {
        state.justPressed = false;
        state.justReleased = false;
    }

    for (auto& [gamepad, buttons] : gamepadButtonStates_) {
        for (auto& [button, state] : buttons) {
            state.justPressed = false;
            state.justReleased = false;
        }
    }

    // Reset mouse delta
    mouseDelta_ = Vector2f(0.0f, 0.0f);
}

void InputManager::processInputQueue() {
    while (!inputQueue_.empty()) {
        const auto& event = inputQueue_.front();

        switch (event.type) {
            case InputType::Keyboard:
                handleKeyboardEvent(std::get<KeyboardEvent>(event.data));
                break;
            case InputType::Mouse:
                handleMouseEvent(std::get<MouseEvent>(event.data));
                break;
            case InputType::Gamepad:
                handleGamepadEvent(std::get<GamepadEvent>(event.data));
                break;
        }

        // Notify callbacks
        for (const auto& callback : inputCallbacks_) {
            callback(event);
        }

        inputQueue_.pop();
    }
}

void InputManager::handleKeyboardEvent(const KeyboardEvent& event) {
    auto& state = keyStates_[event.key];

    switch (event.action) {
        case InputAction::Press:
            state.pressed = true;
            state.justPressed = true;
            break;
        case InputAction::Release:
            state.pressed = false;
            state.justReleased = true;
            break;
        case InputAction::Repeat:
            state.justPressed = true;
            break;
    }
}

void InputManager::handleMouseEvent(const MouseEvent& event) {
    Vector2f newPos(event.xpos, event.ypos);
    mouseDelta_ = newPos - mousePosition_;
    mousePosition_ = newPos;

    if (event.action == InputAction::Press || event.action == InputAction::Release) {
        auto& state = mouseButtonStates_[event.button];

        if (event.action == InputAction::Press) {
            state.pressed = true;
            state.justPressed = true;
        } else {
            state.pressed = false;
            state.justReleased = true;
        }
    }
}

void InputManager::handleGamepadEvent(const GamepadEvent& event) {
    if (event.action == InputAction::Press || event.action == InputAction::Release) {
        auto& state = gamepadButtonStates_[event.gamepad][event.button];

        if (event.action == InputAction::Press) {
            state.pressed = true;
            state.justPressed = true;
        } else {
            state.pressed = false;
            state.justReleased = true;
        }
    } else if (event.action == InputAction::Axis) {
        gamepadAxes_[event.gamepad][event.button] = event.value;
    }
}

void InputManager::setInputFocus(bool focused) {
    if (inputFocus_ != focused) {
        inputFocus_ = focused;
        if (!focused) {
            clearInputBuffer();
        }
        Logger::info("Input focus changed: {}", focused);
    }
}

bool InputManager::hasInputFocus() const {
    return inputFocus_;
}

void InputManager::addInputCallback(InputCallback callback) {
    inputCallbacks_.push_back(callback);
}

void InputManager::removeInputCallback(InputCallback callback) {
    auto it = std::find(inputCallbacks_.begin(), inputCallbacks_.end(), callback);
    if (it != inputCallbacks_.end()) {
        inputCallbacks_.erase(it);
    }
}

// Public interface implementations
bool InputManager::isKeyPressed(int key) const {
    auto it = keyStates_.find(key);
    return it != keyStates_.end() && it->second.pressed;
}

bool InputManager::isMouseButtonPressed(int button) const {
    auto it = mouseButtonStates_.find(button);
    return it != mouseButtonStates_.end() && it->second.pressed;
}

bool InputManager::isGamepadButtonPressed(int gamepad, int button) const {
    auto it = gamepadButtonStates_.find(gamepad);
    if (it == gamepadButtonStates_.end()) return false;

    auto buttonIt = it->second.find(button);
    return buttonIt != it->second.end() && buttonIt->second.pressed;
}

Vector2f InputManager::getMousePosition() const {
    return mousePosition_;
}

Vector2f InputManager::getMouseDelta() const {
    return mouseDelta_;
}

float InputManager::getGamepadAxis(int gamepad, int axis) const {
    auto it = gamepadAxes_.find(gamepad);
    if (it == gamepadAxes_.end()) return 0.0f;

    auto axisIt = it->second.find(axis);
    return axisIt != it->second.end() ? axisIt->second : 0.0f;
}

} // namespace core
} // namespace void_contingency
```

## Key Concepts Explained

### Input Processing Pipeline

1. **Event Generation**:

   - Raw device input detection
   - Event creation and timestamping
   - Initial event filtering

2. **Event Queue**:

   - Input buffering
   - Event ordering
   - Queue management

3. **Event Processing**:
   - Event interpretation
   - State updates
   - Action mapping

### Input State Management

1. **Button States**:

   - Pressed/Released
   - Just Pressed/Released
   - Hold duration
   - Repeat events

2. **Analog Input**:

   - Axis values
   - Dead zones
   - Input smoothing
   - Value scaling

3. **Combined States**:
   - Multiple button combinations
   - Modifier keys
   - Chord recognition

### Input Mapping System

1. **Action Maps**:

   - Named actions
   - Input bindings
   - Action priorities
   - Context sensitivity

2. **Device Mapping**:

   - Device detection
   - Input translation
   - Device fallbacks

3. **Configuration**:
   - User bindings
   - Profile management
   - Default mappings

## Common Pitfalls

1. **Timing Issues**:

   - Input lag
   - Missed inputs
   - Order dependency

2. **State Inconsistencies**:

   - Stuck inputs
   - Ghost inputs
   - State desync

3. **Device Problems**:

   - Disconnections
   - Hot-plugging
   - Driver issues

4. **Focus Issues**:
   - Window focus
   - Background input
   - Modal dialogs

## Best Practices

1. **Input Processing**:

   - Use event queues
   - Handle timing properly
   - Validate input states

2. **Device Support**:

   - Handle disconnections gracefully
   - Support multiple devices
   - Provide fallbacks

3. **Configuration**:

   - Allow key rebinding
   - Support profiles
   - Validate bindings

4. **Debug Support**:
   - Input visualization
   - State logging
   - Error handling

## Testing

Create test file `tests/unit/core/input/InputManagerTest.cpp`:

```cpp
TEST(InputManagerTest, BasicInput) {
    auto& inputManager = InputManager::getInstance();

    // Test key press
    InputEvent event{
        InputType::Keyboard,
        InputAction::Press,
        KeyboardEvent{KEY_SPACE, 0, InputAction::Press, 0},
        0.0f
    };

    inputManager.handleEvent(event);
    EXPECT_TRUE(inputManager.isKeyPressed(KEY_SPACE));
}

TEST(InputManagerTest, ActionMapping) {
    auto& inputManager = InputManager::getInstance();

    inputManager.registerAction("Jump", "Space");

    // Simulate space key press
    InputEvent event{
        InputType::Keyboard,
        InputAction::Press,
        KeyboardEvent{KEY_SPACE, 0, InputAction::Press, 0},
        0.0f
    };

    inputManager.handleEvent(event);
    EXPECT_TRUE(inputManager.isActionPressed("Jump"));
}
```

## Next Steps

1. Add advanced features:

   - Input prediction
   - Gesture recognition
   - Macro system
   - Input recording/playback

2. Implement input visualization
3. Add input configuration UI
4. Create input debugging tools

## Additional Resources

- [Game Loop Implementation](./game_loop_implementation.md)
- [State Management Tutorial](./state_management.md)
- [Event System Tutorial](./event_system.md)
- [Input Configuration Guide](../guides/input_configuration.md)
