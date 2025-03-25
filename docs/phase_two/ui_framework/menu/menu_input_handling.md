# Implementing Menu Input Handling

## Navigation

- Previous: [Menu Transitions Tutorial](./menu_transitions.md)
- Next: [Menu Persistence Tutorial](./menu_persistence.md)

This tutorial will guide you through implementing menu input handling for "Void Contingency", providing a robust system for handling keyboard, mouse, and touch input in menus.

## Overview

The menu input handling system will:

- Support keyboard navigation
- Handle mouse interactions
- Process touch input
- Manage input focus
- Enable input customization
- Provide input debugging

## Implementation Steps

### 1. Create Menu Input Types

Create `include/ui/menu/MenuInput.hpp`:

```cpp
#pragma once

#include "ui/menu/MenuComponents.hpp"
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>

namespace void_contingency {
namespace ui {

enum class InputType {
    Keyboard,
    Mouse,
    Touch,
    Gamepad,
    Custom
};

enum class InputAction {
    Press,
    Release,
    Hold,
    DoublePress,
    LongPress,
    Drag,
    Swipe,
    Pinch,
    Rotate,
    Custom
};

enum class InputModifier {
    None,
    Shift,
    Control,
    Alt,
    Meta,
    Custom
};

struct InputEvent {
    InputType type{InputType::Keyboard};
    InputAction action{InputAction::Press};
    InputModifier modifier{InputModifier::None};
    std::string key;
    std::string value;
    float x{0.0f};
    float y{0.0f};
    float deltaX{0.0f};
    float deltaY{0.0f};
    float pressure{0.0f};
    float timestamp{0.0f};
    std::string target;
    std::vector<std::string> targets;
    std::unordered_map<std::string, std::string> properties;
};

struct InputBinding {
    std::string id;
    InputType type;
    InputAction action;
    InputModifier modifier;
    std::string key;
    std::string command;
    bool enabled{true};
    bool repeat{false};
    float repeatDelay{0.0f};
    float repeatRate{0.0f};
    std::function<void(const InputEvent&)> callback;
};

class MenuInput {
public:
    MenuInput();
    ~MenuInput();

    // Input identification
    void setId(const std::string& id) { properties_.id = id; }
    const std::string& getId() const { return properties_.id; }

    // Input properties
    void setType(InputType type) { properties_.type = type; }
    InputType getType() const { return properties_.type; }

    void setAction(InputAction action) { properties_.action = action; }
    InputAction getAction() const { return properties_.action; }

    void setModifier(InputModifier modifier) { properties_.modifier = modifier; }
    InputModifier getModifier() const { return properties_.modifier; }

    // Input control
    void enable();
    void disable();
    void reset();
    void clear();

    // Input binding
    void addBinding(const InputBinding& binding);
    void removeBinding(const std::string& id);
    void updateBinding(const std::string& id, const InputBinding& binding);
    const InputBinding& getBinding(const std::string& id) const;

    // Input events
    void setOnInput(std::function<void(const InputEvent&)> callback) { onInput_ = callback; }
    void setOnBindingTriggered(std::function<void(const std::string&)> callback) { onBindingTriggered_ = callback; }

    // Input update
    void update(float deltaTime);

private:
    struct InputProperties {
        std::string id;
        InputType type{InputType::Keyboard};
        InputAction action{InputAction::Press};
        InputModifier modifier{InputModifier::None};
        bool enabled{true};
        bool active{false};
        float lastTriggerTime{0.0f};
        float repeatCount{0.0f};
    };

    InputProperties properties_;
    std::unordered_map<std::string, InputBinding> bindings_;
    std::vector<InputEvent> eventQueue_;
    bool isEnabled_{true};
    bool isActive_{false};

    // Input callbacks
    std::function<void(const InputEvent&)> onInput_;
    std::function<void(const std::string&)> onBindingTriggered_;

    // Helper methods
    void processEvent(const InputEvent& event);
    void handleBinding(const InputBinding& binding);
    void updateBindings(float deltaTime);
    void validateBindings();
    void backupBindings();
    void restoreBindings();
};

} // namespace ui
} // namespace void_contingency
```

### 2. Create Menu Input Manager

Create `include/ui/menu/MenuInputManager.hpp`:

```cpp
#pragma once

#include "ui/menu/MenuInput.hpp"
#include <memory>
#include <vector>
#include <unordered_map>
#include <queue>
#include <functional>

namespace void_contingency {
namespace ui {

class MenuInputManager {
public:
    static MenuInputManager& getInstance();

    // Input management
    void addInput(std::shared_ptr<MenuInput> input);
    void removeInput(const std::string& id);
    void updateInput(const std::string& id, std::shared_ptr<MenuInput> input);
    std::shared_ptr<MenuInput> getInput(const std::string& id) const;
    std::vector<std::shared_ptr<MenuInput>> getAllInputs() const;

    // Input control
    void enableInput(const std::string& id);
    void disableInput(const std::string& id);
    void resetInput(const std::string& id);
    void clearInput(const std::string& id);

    // Input binding
    void addBinding(const std::string& inputId, const InputBinding& binding);
    void removeBinding(const std::string& inputId, const std::string& bindingId);
    void updateBinding(const std::string& inputId, const std::string& bindingId, const InputBinding& binding);

    // Input events
    void setOnInputEvent(std::function<void(const InputEvent&)> callback);
    void setOnBindingTriggered(std::function<void(const std::string&, const std::string&)> callback);

    // Input debugging
    void enableDebugging(bool enable);
    void setDebugLevel(int level);
    void logInput(const std::string& message);

    // Input cleanup
    void cleanup();
    void reset();
    void clear();

private:
    MenuInputManager() = default;
    ~MenuInputManager() = default;
    MenuInputManager(const MenuInputManager&) = delete;
    MenuInputManager& operator=(const MenuInputManager&) = delete;

    // Input storage
    std::unordered_map<std::string, std::shared_ptr<MenuInput>> inputs_;
    std::queue<InputEvent> eventQueue_;
    std::vector<std::string> activeInputs_;
    bool debuggingEnabled_{false};
    int debugLevel_{0};

    // Input callbacks
    std::function<void(const InputEvent&)> onInputEvent_;
    std::function<void(const std::string&, const std::string&)> onBindingTriggered_;

    // Helper methods
    void updateInputs(float deltaTime);
    void handleInputEvents();
    void processEventQueue();
    void validateInputs();
    void cleanupInputResources();
    void backupInputState();
    void restoreInputState();
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create Menu Input View

Create `include/ui/menu/MenuInputView.hpp`:

```cpp
#pragma once

#include "ui/menu/MenuInput.hpp"
#include "ui/components/Component.hpp"
#include <memory>

namespace void_contingency {
namespace ui {

class MenuInputView : public Component {
public:
    MenuInputView();
    ~MenuInputView() override;

    // View control
    void setInput(std::shared_ptr<MenuInput> input);
    void setInputs(const std::vector<std::shared_ptr<MenuInput>>& inputs);

    // Interactive features
    void setInteractive(bool interactive) { interactive_ = interactive; }
    bool isInteractive() const { return interactive_; }

    // View customization
    void setShowBindings(bool show) { showBindings_ = show; }
    void setShowEvents(bool show) { showEvents_ = show; }
    void setShowDebug(bool show) { showDebug_ = show; }
    void setShowControls(bool show) { showControls_ = show; }

protected:
    void render(Renderer& renderer) override;
    void handleInput(const InputEvent& event) override;
    void update() override;

private:
    // View state
    std::shared_ptr<MenuInput> input_;
    std::vector<std::shared_ptr<MenuInput>> inputs_;
    bool interactive_{false};
    bool showBindings_{true};
    bool showEvents_{true};
    bool showDebug_{true};
    bool showControls_{true};

    // Helper methods
    void renderBindings(Renderer& renderer);
    void renderEvents(Renderer& renderer);
    void renderDebug(Renderer& renderer);
    void renderControls(Renderer& renderer);
    void handleInputEvent(const InputEvent& event);
    void updateVisualization();
};

} // namespace ui
} // namespace void_contingency
```

### 4. Create Menu Input Tests

Create `tests/unit/ui/menu/InputTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/menu/MenuInput.hpp"
#include "ui/menu/MenuInputManager.hpp"
#include "ui/menu/MenuInputView.hpp"

using namespace void_contingency::ui;

TEST(MenuInputTest, InputCreation) {
    auto input = std::make_shared<MenuInput>();
    input->setId("test_input");
    input->setType(InputType::Keyboard);
    input->setAction(InputAction::Press);

    EXPECT_EQ(input->getId(), "test_input");
    EXPECT_EQ(input->getType(), InputType::Keyboard);
    EXPECT_EQ(input->getAction(), InputAction::Press);
}

TEST(MenuInputTest, InputBinding) {
    auto input = std::make_shared<MenuInput>();
    input->setId("test_input");

    InputBinding binding;
    binding.id = "test_binding";
    binding.type = InputType::Keyboard;
    binding.action = InputAction::Press;
    binding.key = "Space";
    binding.command = "jump";

    input->addBinding(binding);

    const auto& retrievedBinding = input->getBinding("test_binding");
    EXPECT_EQ(retrievedBinding.id, "test_binding");
    EXPECT_EQ(retrievedBinding.key, "Space");
    EXPECT_EQ(retrievedBinding.command, "jump");
}

TEST(MenuInputManagerTest, InputManagement) {
    auto& manager = MenuInputManager::getInstance();

    // Create test input
    auto input = std::make_shared<MenuInput>();
    input->setId("test_input");
    input->setType(InputType::Keyboard);

    // Add input
    manager.addInput(input);

    // Verify input
    auto retrievedInput = manager.getInput("test_input");
    EXPECT_EQ(retrievedInput->getId(), "test_input");
    EXPECT_EQ(retrievedInput->getType(), InputType::Keyboard);
}

TEST(MenuInputViewTest, ViewRendering) {
    MenuInputView view;

    // Create test input
    auto input = std::make_shared<MenuInput>();
    input->setId("test_input");
    input->setType(InputType::Keyboard);

    // Set input
    view.setInput(input);

    // Verify view state
    EXPECT_FALSE(view.isInteractive());
    view.setInteractive(true);
    EXPECT_TRUE(view.isInteractive());
}
```

## Next Steps

1. Implement menu persistence
2. Add menu debugging
3. Implement menu analytics
4. Add menu optimization

## Common Pitfalls

1. **Input Handling**: Handle input events correctly
2. **Input Binding**: Manage input bindings properly
3. **Input Focus**: Handle input focus correctly
4. **Input State**: Manage input state properly

## Best Practices

1. Use proper input types
2. Implement input binding
3. Handle input events
4. Manage input state
5. Support customization

## Additional Resources

- [Menu Components Tutorial](./menu_components.md)
- [Menu Navigation Tutorial](./menu_navigation.md)
- [Menu State Management Tutorial](./menu_state_management.md)
- [Menu Transitions Tutorial](./menu_transitions.md)
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
- [Menu Persistence Tutorial](./menu_persistence.md)
- [Menu Debugging Tutorial](./menu_debugging.md)
- [Menu Analytics Tutorial](./menu_analytics.md)
- [Menu Optimization Tutorial](./menu_optimization.md)
