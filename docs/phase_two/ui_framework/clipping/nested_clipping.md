# Implementing Nested Clipping

## Navigation

- Previous: [Clipping and Masking Tutorial](./clipping_masking.md)
- Next: [Clipping Optimizations Tutorial](./clipping_optimizations.md)

This tutorial will guide you through implementing nested clipping for "Void Contingency", enabling complex clipping hierarchies and interactions.

## Overview

The nested clipping system will:

- Support hierarchical clipping regions
- Handle transform inheritance
- Enable intersection testing
- Provide clipping region caching
- Support clipping region invalidation

## Implementation Steps

### 1. Create Nested Clipping System

Create `include/ui/clip/NestedClipManager.hpp`:

```cpp
#pragma once

#include "ui/clip/ClipManager.hpp"
#include "ui/Component.hpp"
#include <vector>
#include <memory>
#include <unordered_map>
#include <glm/glm.hpp>

namespace void_contingency {
namespace ui {

struct NestedClipRect : public ClipRect {
    ComponentPtr owner;
    std::vector<NestedClipRect> children;
    glm::mat4 transform;
    bool isDirty{true};
    bool isVisible{true};
};

class NestedClipManager {
public:
    static NestedClipManager& getInstance();

    // Clipping hierarchy management
    void addClipRect(ComponentPtr owner, const ClipRect& rect);
    void removeClipRect(ComponentPtr owner);
    void updateClipRect(ComponentPtr owner, const ClipRect& rect);

    // Transform management
    void setTransform(ComponentPtr owner, const glm::mat4& transform);
    void updateTransforms();

    // Visibility management
    void setVisible(ComponentPtr owner, bool visible);
    bool isVisible(ComponentPtr owner) const;

    // Clipping state
    const NestedClipRect* getClipRect(ComponentPtr owner) const;
    bool isClipped(const glm::vec2& point, ComponentPtr owner) const;

    // Performance optimization
    void invalidateClipRect(ComponentPtr owner);
    void updateDirtyClipRects();

private:
    NestedClipManager() = default;
    ~NestedClipManager() = default;
    NestedClipManager(const NestedClipManager&) = delete;
    NestedClipManager& operator=(const NestedClipManager&) = delete;

    // Clipping hierarchy
    std::unordered_map<ComponentPtr, NestedClipRect> clipRects_;
    std::vector<ComponentPtr> dirtyComponents_;

    // Helper methods
    void updateClipRectTransform(NestedClipRect& rect);
    bool intersects(const NestedClipRect& a, const NestedClipRect& b) const;
    void propagateDirtyState(NestedClipRect& rect);
    void updateClipRectCache(NestedClipRect& rect);
};

} // namespace ui
} // namespace void_contingency
```

### 2. Create Clipping Transform Component

Create `include/ui/components/ClippingTransform.hpp`:

```cpp
#pragma once

#include "ui/Component.hpp"
#include "ui/clip/NestedClipManager.hpp"
#include <memory>

namespace void_contingency {
namespace ui {

class ClippingTransform : public Component {
public:
    using ClippingTransformPtr = std::shared_ptr<ClippingTransform>;

    ClippingTransform();
    virtual ~ClippingTransform() = default;

    // Transform properties
    void setTransform(const glm::mat4& transform);
    const glm::mat4& getTransform() const { return transform_; }

    void setClipRect(const ClipRect& rect);
    const ClipRect& getClipRect() const { return clipRect_; }

    void setVisible(bool visible);
    bool isVisible() const { return visible_; }

    // Component interface
    void update(float deltaTime) override;
    void render() override;
    void handleInput(const InputEvent& event) override;

protected:
    // Transform properties
    glm::mat4 transform_{1.0f};
    ClipRect clipRect_;
    bool visible_{true};

    // Helper methods
    void updateClipRect();
    void updateTransform();
    bool isPointClipped(const glm::vec2& point) const;
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create Clipping Transform Tests

Create `tests/unit/ui/clip/NestedClipTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/clip/NestedClipManager.hpp"
#include "ui/components/ClippingTransform.hpp"

using namespace void_contingency::ui;

TEST(NestedClipManagerTest, HierarchyManagement) {
    auto& manager = NestedClipManager::getInstance();

    // Create test components
    auto parent = std::make_shared<ClippingTransform>();
    auto child = std::make_shared<ClippingTransform>();

    // Setup clip rects
    ClipRect parentRect;
    parentRect.position = glm::vec2(0.0f, 0.0f);
    parentRect.size = glm::vec2(100.0f, 100.0f);

    ClipRect childRect;
    childRect.position = glm::vec2(10.0f, 10.0f);
    childRect.size = glm::vec2(50.0f, 50.0f);

    // Add clip rects
    manager.addClipRect(parent, parentRect);
    manager.addClipRect(child, childRect);

    // Verify hierarchy
    auto parentClip = manager.getClipRect(parent);
    auto childClip = manager.getClipRect(child);
    EXPECT_NE(parentClip, nullptr);
    EXPECT_NE(childClip, nullptr);
}

TEST(NestedClipManagerTest, TransformInheritance) {
    auto& manager = NestedClipManager::getInstance();

    // Create test components
    auto parent = std::make_shared<ClippingTransform>();
    auto child = std::make_shared<ClippingTransform>();

    // Setup transforms
    glm::mat4 parentTransform = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 0.0f));
    glm::mat4 childTransform = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 0.0f));

    // Set transforms
    manager.setTransform(parent, parentTransform);
    manager.setTransform(child, childTransform);

    // Update transforms
    manager.updateTransforms();

    // Verify transform inheritance
    auto childClip = manager.getClipRect(child);
    EXPECT_NE(childClip, nullptr);
    EXPECT_EQ(childClip->transform, parentTransform * childTransform);
}

TEST(NestedClipManagerTest, IntersectionTesting) {
    auto& manager = NestedClipManager::getInstance();

    // Create test components
    auto component = std::make_shared<ClippingTransform>();

    // Setup clip rect
    ClipRect rect;
    rect.position = glm::vec2(0.0f, 0.0f);
    rect.size = glm::vec2(100.0f, 100.0f);

    // Add clip rect
    manager.addClipRect(component, rect);

    // Test point clipping
    glm::vec2 insidePoint(50.0f, 50.0f);
    glm::vec2 outsidePoint(150.0f, 150.0f);

    EXPECT_TRUE(manager.isClipped(insidePoint, component));
    EXPECT_FALSE(manager.isClipped(outsidePoint, component));
}

TEST(ClippingTransformTest, TransformProperties) {
    auto transform = std::make_shared<ClippingTransform>();

    // Set transform properties
    glm::mat4 testTransform = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 0.0f));
    transform->setTransform(testTransform);

    ClipRect testRect;
    testRect.position = glm::vec2(0.0f, 0.0f);
    testRect.size = glm::vec2(100.0f, 100.0f);
    transform->setClipRect(testRect);

    transform->setVisible(false);

    // Verify properties
    EXPECT_EQ(transform->getTransform(), testTransform);
    EXPECT_EQ(transform->getClipRect().position, testRect.position);
    EXPECT_FALSE(transform->isVisible());
}
```

## Next Steps

1. Add performance optimizations
2. Implement clipping region caching
3. Add clipping region debugging
4. Implement clipping region serialization

## Common Pitfalls

1. **Transform Inheritance**: Handle transform composition
2. **Intersection Testing**: Optimize intersection calculations
3. **State Management**: Handle dirty state propagation
4. **Performance**: Optimize clipping region updates

## Best Practices

1. Use transform inheritance
2. Cache clipping regions
3. Optimize intersection tests
4. Handle state invalidation
5. Support debugging tools

## Additional Resources

- [Clipping and Masking Tutorial](./clipping_masking.md)
- [Clipping Optimizations Tutorial](./clipping_optimizations.md)
- [Clipping Debugging Tutorial](./clipping_debugging.md)
- [Clipping Serialization Tutorial](./clipping_serialization.md)
- [Clipping Compression Tutorial](./clipping_compression.md)
