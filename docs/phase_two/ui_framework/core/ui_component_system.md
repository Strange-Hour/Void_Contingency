# Implementing UI Component System

## Navigation

- Previous: [Upgrade System Tutorial](../../game_loop/upgrade_system.md)
- Next: [Layout Management Tutorial](./layout_management.md)

This tutorial will guide you through implementing the UI component system for "Void Contingency", providing the foundation for all UI elements.

## Overview

The UI component system will:

- Handle UI component hierarchy and composition
- Manage component lifecycle and updates
- Support component styling and layout
- Handle input events and focus
- Provide component state management
- Support component animations and transitions

## Implementation Steps

### 1. Create UI Component Base Class

Create `include/ui/Component.hpp`:

```cpp
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include "core/event/Event.hpp"

namespace void_contingency {
namespace ui {

class Component {
public:
    using ComponentPtr = std::shared_ptr<Component>;
    using EventCallback = std::function<void(const core::Event&)>;

    Component(const std::string& id = "");
    virtual ~Component() = default;

    // Component lifecycle
    virtual void initialize();
    virtual void update(float deltaTime);
    virtual void render();
    virtual void cleanup();

    // Component hierarchy
    void addChild(ComponentPtr child);
    void removeChild(ComponentPtr child);
    void setParent(ComponentPtr parent);
    ComponentPtr getParent() const { return parent_; }
    const std::vector<ComponentPtr>& getChildren() const { return children_; }

    // Component properties
    void setId(const std::string& id) { id_ = id; }
    const std::string& getId() const { return id_; }
    void setVisible(bool visible) { visible_ = visible; }
    bool isVisible() const { return visible_; }
    void setEnabled(bool enabled) { enabled_ = enabled; }
    bool isEnabled() const { return enabled_; }

    // Event handling
    void addEventListener(const std::string& type, EventCallback callback);
    void removeEventListener(const std::string& type, EventCallback callback);
    virtual bool handleEvent(const core::Event& event);

    // Focus management
    virtual bool canReceiveFocus() const { return false; }
    virtual void onFocusGained();
    virtual void onFocusLost();

    // Layout properties
    void setPosition(float x, float y);
    void setSize(float width, float height);
    void setAnchor(float left, float top, float right, float bottom);
    void setMargin(float left, float top, float right, float bottom);
    void setPadding(float left, float top, float right, float bottom);

    // Style properties
    void setBackgroundColor(const glm::vec4& color);
    void setBorderColor(const glm::vec4& color);
    void setBorderWidth(float width);
    void setBorderRadius(float radius);
    void setOpacity(float opacity);

    // Layout calculations
    virtual void calculateLayout();
    virtual void applyLayout();

protected:
    // Component state
    std::string id_;
    ComponentPtr parent_;
    std::vector<ComponentPtr> children_;
    bool visible_{true};
    bool enabled_{true};
    bool focused_{false};

    // Layout properties
    struct Layout {
        glm::vec2 position{0.0f};
        glm::vec2 size{0.0f};
        glm::vec4 anchor{0.0f};
        glm::vec4 margin{0.0f};
        glm::vec4 padding{0.0f};
        glm::vec2 calculatedPosition{0.0f};
        glm::vec2 calculatedSize{0.0f};
    } layout_;

    // Style properties
    struct Style {
        glm::vec4 backgroundColor{1.0f};
        glm::vec4 borderColor{0.0f};
        float borderWidth{0.0f};
        float borderRadius{0.0f};
        float opacity{1.0f};
    } style_;

    // Event handling
    std::unordered_map<std::string, std::vector<EventCallback>> eventListeners_;

    // Helper methods
    void notifyEventListeners(const core::Event& event);
    void updateChildren(float deltaTime);
    void renderChildren();
    void cleanupChildren();
    bool isPointInside(const glm::vec2& point) const;
};

} // namespace ui
} // namespace void_contingency
```

### 2. Implement UI Component Base Class

Create `src/ui/Component.cpp`:

```cpp
#include "ui/Component.hpp"
#include "core/Logger.hpp"
#include <algorithm>

namespace void_contingency {
namespace ui {

Component::Component(const std::string& id)
    : id_(id)
{
}

void Component::initialize() {
    for (auto& child : children_) {
        child->initialize();
    }
}

void Component::update(float deltaTime) {
    if (!enabled_ || !visible_) {
        return;
    }

    updateChildren(deltaTime);
}

void Component::render() {
    if (!visible_) {
        return;
    }

    // Render component background
    if (style_.backgroundColor.a > 0.0f) {
        // TODO: Implement background rendering
    }

    // Render component border
    if (style_.borderWidth > 0.0f) {
        // TODO: Implement border rendering
    }

    renderChildren();
}

void Component::cleanup() {
    cleanupChildren();
    eventListeners_.clear();
}

void Component::addChild(ComponentPtr child) {
    if (!child) {
        Logger::warning("Attempted to add null child component");
        return;
    }

    if (child->getParent()) {
        child->getParent()->removeChild(child);
    }

    children_.push_back(child);
    child->setParent(shared_from_this());
    Logger::debug("Added child component: {}", child->getId());
}

void Component::removeChild(ComponentPtr child) {
    auto it = std::find(children_.begin(), children_.end(), child);
    if (it != children_.end()) {
        (*it)->setParent(nullptr);
        children_.erase(it);
        Logger::debug("Removed child component: {}", child->getId());
    }
}

void Component::setParent(ComponentPtr parent) {
    parent_ = parent;
}

void Component::addEventListener(const std::string& type, EventCallback callback) {
    eventListeners_[type].push_back(callback);
    Logger::debug("Added event listener for type: {} to component: {}", type, id_);
}

void Component::removeEventListener(const std::string& type, EventCallback callback) {
    auto it = eventListeners_.find(type);
    if (it != eventListeners_.end()) {
        auto& callbacks = it->second;
        callbacks.erase(
            std::remove_if(callbacks.begin(), callbacks.end(),
                [&callback](const EventCallback& c) { return c.target_type() == callback.target_type(); }),
            callbacks.end()
        );
        Logger::debug("Removed event listener for type: {} from component: {}", type, id_);
    }
}

bool Component::handleEvent(const core::Event& event) {
    if (!enabled_ || !visible_) {
        return false;
    }

    notifyEventListeners(event);

    // Handle event in reverse order (top to bottom)
    for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
        if ((*it)->handleEvent(event)) {
            return true;
        }
    }

    return false;
}

void Component::onFocusGained() {
    focused_ = true;
    Logger::debug("Component gained focus: {}", id_);
}

void Component::onFocusLost() {
    focused_ = false;
    Logger::debug("Component lost focus: {}", id_);
}

void Component::setPosition(float x, float y) {
    layout_.position = glm::vec2(x, y);
    calculateLayout();
}

void Component::setSize(float width, float height) {
    layout_.size = glm::vec2(width, height);
    calculateLayout();
}

void Component::setAnchor(float left, float top, float right, float bottom) {
    layout_.anchor = glm::vec4(left, top, right, bottom);
    calculateLayout();
}

void Component::setMargin(float left, float top, float right, float bottom) {
    layout_.margin = glm::vec4(left, top, right, bottom);
    calculateLayout();
}

void Component::setPadding(float left, float top, float right, float bottom) {
    layout_.padding = glm::vec4(left, top, right, bottom);
    calculateLayout();
}

void Component::setBackgroundColor(const glm::vec4& color) {
    style_.backgroundColor = color;
}

void Component::setBorderColor(const glm::vec4& color) {
    style_.borderColor = color;
}

void Component::setBorderWidth(float width) {
    style_.borderWidth = width;
}

void Component::setBorderRadius(float radius) {
    style_.borderRadius = radius;
}

void Component::setOpacity(float opacity) {
    style_.opacity = glm::clamp(opacity, 0.0f, 1.0f);
}

void Component::calculateLayout() {
    if (!parent_) {
        layout_.calculatedPosition = layout_.position;
        layout_.calculatedSize = layout_.size;
        return;
    }

    // Calculate position based on anchor and parent size
    glm::vec2 parentSize = parent_->layout_.calculatedSize;
    layout_.calculatedPosition = layout_.position +
        glm::vec2(
            parentSize.x * layout_.anchor.x,
            parentSize.y * layout_.anchor.y
        );

    // Calculate size based on parent size and margins
    layout_.calculatedSize = layout_.size;
    if (layout_.size.x == 0.0f) {
        layout_.calculatedSize.x = parentSize.x * (1.0f - layout_.anchor.x - layout_.anchor.z);
    }
    if (layout_.size.y == 0.0f) {
        layout_.calculatedSize.y = parentSize.y * (1.0f - layout_.anchor.y - layout_.anchor.w);
    }

    // Apply margins
    layout_.calculatedSize -= glm::vec2(
        layout_.margin.x + layout_.margin.z,
        layout_.margin.y + layout_.margin.w
    );

    // Apply layout to children
    for (auto& child : children_) {
        child->calculateLayout();
    }
}

void Component::applyLayout() {
    // Apply layout to children
    for (auto& child : children_) {
        child->applyLayout();
    }
}

void Component::notifyEventListeners(const core::Event& event) {
    auto it = eventListeners_.find(event.getType());
    if (it != eventListeners_.end()) {
        for (const auto& callback : it->second) {
            try {
                callback(event);
            } catch (const std::exception& e) {
                Logger::error("Error in event listener: {}", e.what());
            }
        }
    }
}

void Component::updateChildren(float deltaTime) {
    for (auto& child : children_) {
        child->update(deltaTime);
    }
}

void Component::renderChildren() {
    for (auto& child : children_) {
        child->render();
    }
}

void Component::cleanupChildren() {
    for (auto& child : children_) {
        child->cleanup();
    }
}

bool Component::isPointInside(const glm::vec2& point) const {
    return point.x >= layout_.calculatedPosition.x &&
           point.x <= layout_.calculatedPosition.x + layout_.calculatedSize.x &&
           point.y >= layout_.calculatedPosition.y &&
           point.y <= layout_.calculatedPosition.y + layout_.calculatedSize.y;
}

} // namespace ui
} // namespace void_contingency
```

### 3. Create UI Component Tests

Create `tests/unit/ui/ComponentTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/Component.hpp"

using namespace void_contingency::ui;

class TestComponent : public Component {
public:
    TestComponent(const std::string& id = "") : Component(id) {}
    bool updateCalled{false};
    bool renderCalled{false};
    bool cleanupCalled{false};

    void update(float deltaTime) override {
        updateCalled = true;
        Component::update(deltaTime);
    }

    void render() override {
        renderCalled = true;
        Component::render();
    }

    void cleanup() override {
        cleanupCalled = true;
        Component::cleanup();
    }
};

TEST(ComponentTest, ComponentCreation) {
    auto component = std::make_shared<TestComponent>("test");

    EXPECT_EQ(component->getId(), "test");
    EXPECT_TRUE(component->isVisible());
    EXPECT_TRUE(component->isEnabled());
    EXPECT_EQ(component->getChildren().size(), 0);
    EXPECT_EQ(component->getParent(), nullptr);
}

TEST(ComponentTest, ComponentHierarchy) {
    auto parent = std::make_shared<TestComponent>("parent");
    auto child = std::make_shared<TestComponent>("child");

    parent->addChild(child);

    EXPECT_EQ(parent->getChildren().size(), 1);
    EXPECT_EQ(child->getParent(), parent);

    parent->removeChild(child);

    EXPECT_EQ(parent->getChildren().size(), 0);
    EXPECT_EQ(child->getParent(), nullptr);
}

TEST(ComponentTest, ComponentLifecycle) {
    auto component = std::make_shared<TestComponent>("test");

    component->initialize();
    component->update(0.1f);
    component->render();
    component->cleanup();

    EXPECT_TRUE(component->updateCalled);
    EXPECT_TRUE(component->renderCalled);
    EXPECT_TRUE(component->cleanupCalled);
}

TEST(ComponentTest, ComponentLayout) {
    auto parent = std::make_shared<TestComponent>("parent");
    auto child = std::make_shared<TestComponent>("child");

    parent->setSize(100.0f, 100.0f);
    child->setAnchor(0.0f, 0.0f, 0.5f, 0.5f);

    parent->addChild(child);
    parent->calculateLayout();

    // Child should be positioned at (0,0) and sized 50x50
    EXPECT_EQ(child->getLayout().calculatedPosition.x, 0.0f);
    EXPECT_EQ(child->getLayout().calculatedPosition.y, 0.0f);
    EXPECT_EQ(child->getLayout().calculatedSize.x, 50.0f);
    EXPECT_EQ(child->getLayout().calculatedSize.y, 50.0f);
}

TEST(ComponentTest, ComponentVisibility) {
    auto component = std::make_shared<TestComponent>("test");

    component->setVisible(false);
    EXPECT_FALSE(component->isVisible());

    component->setVisible(true);
    EXPECT_TRUE(component->isVisible());
}

TEST(ComponentTest, ComponentEnabled) {
    auto component = std::make_shared<TestComponent>("test");

    component->setEnabled(false);
    EXPECT_FALSE(component->isEnabled());

    component->setEnabled(true);
    EXPECT_TRUE(component->isEnabled());
}
```

## Next Steps

1. Implement specific UI components (Button, Text, Image, etc.)
2. Add layout management system
3. Implement styling system
4. Add input handling system

## Common Pitfalls

1. **Component Lifecycle**: Handle initialization and cleanup properly
2. **Memory Management**: Use smart pointers for component ownership
3. **Event Propagation**: Handle event bubbling correctly
4. **Layout Calculation**: Handle complex layout scenarios

## Best Practices

1. Use component composition
2. Implement proper event handling
3. Handle focus management
4. Support component styling
5. Include layout calculations

## Additional Resources

- [Upgrade System Tutorial](../../game_loop/upgrade_system.md)
- [Layout Management Tutorial](./layout_management.md)
- [Styling System Tutorial](./styling_system.md)
- [Component Rendering Tutorial](./component_rendering.md)
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
- [HUD Elements Tutorial](../hud/hud_elements.md)
