# Implementing Menu Components

## Navigation

- Previous: [Menu System Tutorial](./menu_system.md)
- Next: [Menu Navigation Tutorial](./menu_navigation.md)

This tutorial will guide you through implementing the core menu components for "Void Contingency", providing the building blocks for creating interactive menus.

## Overview

The menu components system will:

- Define basic menu elements
- Support menu containers
- Enable interactive components
- Handle menu layouts
- Manage menu states
- Support menu styling

## Implementation Steps

### 1. Create Menu Component Types

Create `include/ui/menu/MenuComponents.hpp`:

```cpp
#pragma once

#include "ui/components/Component.hpp"
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>

namespace void_contingency {
namespace ui {

enum class MenuComponentType {
    Button,
    Label,
    TextBox,
    CheckBox,
    RadioButton,
    Slider,
    DropDown,
    ListBox,
    TabView,
    Panel,
    GroupBox,
    Separator,
    Spacer,
    Image,
    Icon,
    ProgressBar,
    ScrollBar,
    ToolTip,
    ContextMenu
};

struct MenuComponentStyle {
    Color backgroundColor{Color::Transparent};
    Color borderColor{Color::White};
    float borderWidth{1.0f};
    float cornerRadius{4.0f};
    Color textColor{Color::White};
    Color hoverColor{Color(0.8f, 0.8f, 0.8f, 1.0f)};
    Color pressedColor{Color(0.6f, 0.6f, 0.6f, 1.0f)};
    Color disabledColor{Color(0.5f, 0.5f, 0.5f, 1.0f)};
    Font font;
    float fontSize{16.0f};
    TextAlignment textAlignment{TextAlignment::Left};
    Padding padding{4.0f};
    Margin margin{0.0f};
    bool isVisible{true};
    bool isEnabled{true};
    bool isFocused{false};
    bool isHovered{false};
    bool isPressed{false};
    bool isSelected{false};
    bool isChecked{false};
    bool isExpanded{false};
    bool isScrollable{false};
    bool isResizable{false};
    bool isDraggable{false};
    bool isDroppable{false};
    bool isModal{false};
    bool isPopup{false};
    bool isTooltip{false};
    bool isContextMenu{false};
    std::string tooltip;
    std::string contextMenu;
    std::string shortcut;
    std::string icon;
    std::string image;
    std::string sound;
    std::string animation;
    std::unordered_map<std::string, std::string> customProperties;
};

class MenuComponent : public Component {
public:
    MenuComponent();
    ~MenuComponent() override;

    // Component identification
    void setId(const std::string& id) { id_ = id; }
    const std::string& getId() const { return id_; }

    void setName(const std::string& name) { name_ = name; }
    const std::string& getName() const { return name_; }

    void setType(MenuComponentType type) { type_ = type; }
    MenuComponentType getType() const { return type_; }

    // Component hierarchy
    void setParent(MenuComponent* parent) { parent_ = parent; }
    MenuComponent* getParent() const { return parent_; }

    void addChild(std::shared_ptr<MenuComponent> child);
    void removeChild(const std::string& id);
    std::shared_ptr<MenuComponent> getChild(const std::string& id) const;
    const std::vector<std::shared_ptr<MenuComponent>>& getChildren() const { return children_; }

    // Component styling
    void setStyle(const MenuComponentStyle& style) { style_ = style; }
    const MenuComponentStyle& getStyle() const { return style_; }

    // Component state
    void setVisible(bool visible) { style_.isVisible = visible; }
    bool isVisible() const { return style_.isVisible; }

    void setEnabled(bool enabled) { style_.isEnabled = enabled; }
    bool isEnabled() const { return style_.isEnabled; }

    void setFocused(bool focused) { style_.isFocused = focused; }
    bool isFocused() const { return style_.isFocused; }

    void setHovered(bool hovered) { style_.isHovered = hovered; }
    bool isHovered() const { return style_.isHovered; }

    void setPressed(bool pressed) { style_.isPressed = pressed; }
    bool isPressed() const { return style_.isPressed; }

    void setSelected(bool selected) { style_.isSelected = selected; }
    bool isSelected() const { return style_.isSelected; }

    void setChecked(bool checked) { style_.isChecked = checked; }
    bool isChecked() const { return style_.isChecked; }

    void setExpanded(bool expanded) { style_.isExpanded = expanded; }
    bool isExpanded() const { return style_.isExpanded; }

    // Component interaction
    void setOnClick(std::function<void()> callback) { onClick_ = callback; }
    void setOnHover(std::function<void()> callback) { onHover_ = callback; }
    void setOnPress(std::function<void()> callback) { onPress_ = callback; }
    void setOnRelease(std::function<void()> callback) { onRelease_ = callback; }
    void setOnFocus(std::function<void()> callback) { onFocus_ = callback; }
    void setOnBlur(std::function<void()> callback) { onBlur_ = callback; }
    void setOnSelect(std::function<void()> callback) { onSelect_ = callback; }
    void setOnDeselect(std::function<void()> callback) { onDeselect_ = callback; }
    void setOnCheck(std::function<void()> callback) { onCheck_ = callback; }
    void setOnUncheck(std::function<void()> callback) { onUncheck_ = callback; }
    void setOnExpand(std::function<void()> callback) { onExpand_ = callback; }
    void setOnCollapse(std::function<void()> callback) { onCollapse_ = callback; }

    // Component layout
    void setPosition(const Vector2f& position) { position_ = position; }
    const Vector2f& getPosition() const { return position_; }

    void setSize(const Vector2f& size) { size_ = size; }
    const Vector2f& getSize() const { return size_; }

    void setMinSize(const Vector2f& size) { minSize_ = size; }
    const Vector2f& getMinSize() const { return minSize_; }

    void setMaxSize(const Vector2f& size) { maxSize_ = size; }
    const Vector2f& getMaxSize() const { return maxSize_; }

    void setPadding(const Padding& padding) { style_.padding = padding; }
    const Padding& getPadding() const { return style_.padding; }

    void setMargin(const Margin& margin) { style_.margin = margin; }
    const Margin& getMargin() const { return style_.margin; }

    // Component rendering
    void render(Renderer& renderer) override;
    void handleInput(const InputEvent& event) override;
    void update() override;

protected:
    // Component state
    std::string id_;
    std::string name_;
    MenuComponentType type_{MenuComponentType::Panel};
    MenuComponent* parent_{nullptr};
    std::vector<std::shared_ptr<MenuComponent>> children_;
    MenuComponentStyle style_;
    Vector2f position_;
    Vector2f size_;
    Vector2f minSize_;
    Vector2f maxSize_;

    // Component callbacks
    std::function<void()> onClick_;
    std::function<void()> onHover_;
    std::function<void()> onPress_;
    std::function<void()> onRelease_;
    std::function<void()> onFocus_;
    std::function<void()> onBlur_;
    std::function<void()> onSelect_;
    std::function<void()> onDeselect_;
    std::function<void()> onCheck_;
    std::function<void()> onUncheck_;
    std::function<void()> onExpand_;
    std::function<void()> onCollapse_;

    // Helper methods
    void updateLayout();
    void updateStyle();
    void updateState();
    void handleMouseInput(const MouseEvent& event);
    void handleKeyboardInput(const KeyboardEvent& event);
    void handleTouchInput(const TouchEvent& event);
    void handleGestureInput(const GestureEvent& event);
    void handleFocusInput(const FocusEvent& event);
    void handleResizeInput(const ResizeEvent& event);
    void handleDragInput(const DragEvent& event);
    void handleDropInput(const DropEvent& event);
    void handleScrollInput(const ScrollEvent& event);
    void handleContextMenuInput(const ContextMenuEvent& event);
    void handleTooltipInput(const TooltipEvent& event);
    void handleAnimationInput(const AnimationEvent& event);
    void handleTransitionInput(const TransitionEvent& event);
    void handleStateInput(const StateEvent& event);
    void handlePropertyInput(const PropertyEvent& event);
    void handleCustomInput(const CustomEvent& event);
    void notifyParent();
    void notifyChildren();
    void cleanupResources();
    void validateState();
    void backupState();
    void restoreState();
};

} // namespace ui
} // namespace void_contingency
```

### 2. Create Menu Component Manager

Create `include/ui/menu/MenuComponentManager.hpp`:

```cpp
#pragma once

#include "ui/menu/MenuComponents.hpp"
#include <memory>
#include <vector>
#include <unordered_map>

namespace void_contingency {
namespace ui {

class MenuComponentManager {
public:
    static MenuComponentManager& getInstance();

    // Component management
    void addComponent(std::shared_ptr<MenuComponent> component);
    void removeComponent(const std::string& id);
    void updateComponent(const std::string& id, std::shared_ptr<MenuComponent> component);
    std::shared_ptr<MenuComponent> getComponent(const std::string& id) const;
    std::vector<std::shared_ptr<MenuComponent>> getAllComponents() const;

    // Component hierarchy
    void setParent(const std::string& childId, const std::string& parentId);
    void removeParent(const std::string& childId);
    std::shared_ptr<MenuComponent> getParent(const std::string& id) const;
    std::vector<std::shared_ptr<MenuComponent>> getChildren(const std::string& id) const;

    // Component state
    void setVisible(const std::string& id, bool visible);
    void setEnabled(const std::string& id, bool enabled);
    void setFocused(const std::string& id, bool focused);
    void setSelected(const std::string& id, bool selected);
    void setChecked(const std::string& id, bool checked);
    void setExpanded(const std::string& id, bool expanded);

    // Component interaction
    void handleInput(const InputEvent& event);
    void update();
    void render(Renderer& renderer);

    // Component cleanup
    void cleanup();
    void reset();
    void clear();

private:
    MenuComponentManager() = default;
    ~MenuComponentManager() = default;
    MenuComponentManager(const MenuComponentManager&) = delete;
    MenuComponentManager& operator=(const MenuComponentManager&) = delete;

    // Component storage
    std::unordered_map<std::string, std::shared_ptr<MenuComponent>> components_;
    std::shared_ptr<MenuComponent> focusedComponent_;
    std::shared_ptr<MenuComponent> selectedComponent_;
    std::shared_ptr<MenuComponent> hoveredComponent_;
    std::shared_ptr<MenuComponent> pressedComponent_;

    // Helper methods
    void updateComponentState();
    void updateComponentLayout();
    void updateComponentStyle();
    void handleComponentInput(const InputEvent& event);
    void notifyComponentCallbacks();
    void cleanupComponentResources();
    void validateComponentState();
    void backupComponentState();
    void restoreComponentState();
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create Menu Component View

Create `include/ui/menu/MenuComponentView.hpp`:

```cpp
#pragma once

#include "ui/menu/MenuComponents.hpp"
#include "ui/components/Component.hpp"
#include <memory>

namespace void_contingency {
namespace ui {

class MenuComponentView : public Component {
public:
    MenuComponentView();
    ~MenuComponentView() override;

    // View control
    void setComponent(std::shared_ptr<MenuComponent> component);
    void setComponents(const std::vector<std::shared_ptr<MenuComponent>>& components);

    // Interactive features
    void setInteractive(bool interactive) { interactive_ = interactive; }
    bool isInteractive() const { return interactive_; }

    // View customization
    void setShowHierarchy(bool show) { showHierarchy_ = show; }
    void setShowProperties(bool show) { showProperties_ = show; }
    void setShowStyle(bool show) { showStyle_ = show; }
    void setShowState(bool show) { showState_ = show; }
    void setShowControls(bool show) { showControls_ = show; }

protected:
    void render(Renderer& renderer) override;
    void handleInput(const InputEvent& event) override;
    void update() override;

private:
    // View state
    std::shared_ptr<MenuComponent> component_;
    std::vector<std::shared_ptr<MenuComponent>> components_;
    bool interactive_{false};
    bool showHierarchy_{true};
    bool showProperties_{true};
    bool showStyle_{true};
    bool showState_{true};
    bool showControls_{true};

    // Helper methods
    void renderHierarchy(Renderer& renderer);
    void renderProperties(Renderer& renderer);
    void renderStyle(Renderer& renderer);
    void renderState(Renderer& renderer);
    void renderControls(Renderer& renderer);
    void handleComponentInput(const InputEvent& event);
    void updateVisualization();
};

} // namespace ui
} // namespace void_contingency
```

### 4. Create Menu Component Tests

Create `tests/unit/ui/menu/ComponentTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/menu/MenuComponents.hpp"
#include "ui/menu/MenuComponentManager.hpp"
#include "ui/menu/MenuComponentView.hpp"

using namespace void_contingency::ui;

TEST(MenuComponentTest, ComponentCreation) {
    auto component = std::make_shared<MenuComponent>();
    component->setId("test_component");
    component->setName("Test Component");
    component->setType(MenuComponentType::Panel);

    EXPECT_EQ(component->getId(), "test_component");
    EXPECT_EQ(component->getName(), "Test Component");
    EXPECT_EQ(component->getType(), MenuComponentType::Panel);
}

TEST(MenuComponentTest, ComponentHierarchy) {
    auto parent = std::make_shared<MenuComponent>();
    parent->setId("parent");
    auto child = std::make_shared<MenuComponent>();
    child->setId("child");

    parent->addChild(child);
    child->setParent(parent.get());

    EXPECT_EQ(child->getParent(), parent.get());
    EXPECT_EQ(parent->getChild("child"), child);
}

TEST(MenuComponentTest, ComponentState) {
    auto component = std::make_shared<MenuComponent>();
    component->setId("test_component");

    component->setVisible(false);
    EXPECT_FALSE(component->isVisible());

    component->setEnabled(false);
    EXPECT_FALSE(component->isEnabled());

    component->setFocused(true);
    EXPECT_TRUE(component->isFocused());

    component->setSelected(true);
    EXPECT_TRUE(component->isSelected());
}

TEST(MenuComponentManagerTest, ComponentManagement) {
    auto& manager = MenuComponentManager::getInstance();

    // Create test component
    auto component = std::make_shared<MenuComponent>();
    component->setId("test_component");

    // Add component
    manager.addComponent(component);

    // Verify component
    auto retrievedComponent = manager.getComponent("test_component");
    EXPECT_EQ(retrievedComponent->getId(), "test_component");
}

TEST(MenuComponentViewTest, ViewRendering) {
    MenuComponentView view;

    // Create test component
    auto component = std::make_shared<MenuComponent>();
    component->setId("test_component");
    component->setName("Test Component");

    // Set component
    view.setComponent(component);

    // Verify view state
    EXPECT_FALSE(view.isInteractive());
    view.setInteractive(true);
    EXPECT_TRUE(view.isInteractive());
}
```

## Next Steps

1. Implement menu navigation
2. Add menu state management
3. Implement menu transitions
4. Add menu input handling

## Common Pitfalls

1. **Component Hierarchy**: Maintain proper parent-child relationships
2. **State Management**: Handle component states correctly
3. **Event Handling**: Process input events properly
4. **Resource Management**: Clean up resources appropriately

## Best Practices

1. Use proper component hierarchy
2. Implement state management
3. Handle input events
4. Manage resources
5. Support customization

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
- [HUD Styling Tutorial](./hud_styling.md)
- [HUD Persistence Tutorial](./hud_persistence.md)
- [HUD Debugging Tutorial](./hud_debugging.md)
- [HUD Analytics Tutorial](./hud_analytics.md)
- [HUD Optimization Tutorial](./hud_optimization.md)
- [HUD Sharing Tutorial](./hud_sharing.md)
- [HUD Testing Tutorial](./hud_testing.md)
- [HUD Profiling Tutorial](./hud_profiling.md)
- [HUD Benchmarking Tutorial](./hud_benchmarking.md)
