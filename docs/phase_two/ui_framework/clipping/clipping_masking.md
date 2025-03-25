# Implementing Clipping and Masking

## Navigation

- Previous: [Theme System Tutorial](../theme/theme_system.md)
- Next: [Nested Clipping Tutorial](./nested_clipping.md)

This tutorial will guide you through implementing clipping and masking for "Void Contingency", providing tools for controlling UI element visibility and rendering.

## Overview

The clipping and masking system will:

- Support scissor testing
- Enable stencil masking
- Handle alpha masking
- Provide clipping regions
- Support nested clipping

## Implementation Steps

### 1. Create Clipping System

Create `include/ui/clip/ClipManager.hpp`:

```cpp
#pragma once

#include "ui/Component.hpp"
#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace void_contingency {
namespace ui {

struct ClipRect {
    glm::vec2 position;
    glm::vec2 size;
    float rotation{0.0f};
    bool enabled{true};
};

class ClipManager {
public:
    static ClipManager& getInstance();

    // Clipping management
    void pushClipRect(const ClipRect& rect);
    void popClipRect();
    void clearClipRects();

    // Scissor testing
    void enableScissorTest();
    void disableScissorTest();
    void setScissorRect(const glm::vec2& position, const glm::vec2& size);

    // Stencil masking
    void enableStencilTest();
    void disableStencilTest();
    void clearStencilBuffer();
    void setStencilMask(uint32_t mask);

    // Alpha masking
    void enableAlphaTest();
    void disableAlphaTest();
    void setAlphaThreshold(float threshold);

    // Clipping state
    bool isClippingEnabled() const { return clippingEnabled_; }
    const std::vector<ClipRect>& getClipRects() const { return clipRects_; }

private:
    ClipManager() = default;
    ~ClipManager() = default;
    ClipManager(const ClipManager&) = delete;
    ClipManager& operator=(const ClipManager&) = delete;

    // Clipping state
    std::vector<ClipRect> clipRects_;
    bool clippingEnabled_{false};

    // Scissor state
    bool scissorEnabled_{false};
    glm::vec2 scissorPosition_;
    glm::vec2 scissorSize_;

    // Stencil state
    bool stencilEnabled_{false};
    uint32_t stencilMask_{0xFF};

    // Alpha state
    bool alphaEnabled_{false};
    float alphaThreshold_{0.1f};

    // Helper methods
    void updateScissorRect();
    void updateStencilMask();
    void updateAlphaTest();
};

} // namespace ui
} // namespace void_contingency
```

### 2. Create Mask Component

Create `include/ui/components/Mask.hpp`:

```cpp
#pragma once

#include "ui/Component.hpp"
#include "ui/clip/ClipManager.hpp"
#include <memory>

namespace void_contingency {
namespace ui {

class Mask : public Component {
public:
    using MaskPtr = std::shared_ptr<Mask>;

    Mask();
    virtual ~Mask() = default;

    // Mask properties
    void setMaskType(MaskType type) { maskType_ = type; }
    MaskType getMaskType() const { return maskType_; }

    void setInverted(bool inverted) { inverted_ = inverted; }
    bool isInverted() const { return inverted_; }

    void setAlphaThreshold(float threshold) { alphaThreshold_ = threshold; }
    float getAlphaThreshold() const { return alphaThreshold_; }

    // Component interface
    void update(float deltaTime) override;
    void render() override;
    void handleInput(const InputEvent& event) override;

protected:
    // Mask properties
    MaskType maskType_{MaskType::Alpha};
    bool inverted_{false};
    float alphaThreshold_{0.1f};

    // Helper methods
    void setupMask();
    void cleanupMask();
    void renderMask();
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create Clipping Shader

Create `include/ui/render/shaders/ClippingShader.hpp`:

```cpp
#pragma once

#include "ui/render/Shader.hpp"
#include <string>

namespace void_contingency {
namespace ui {

class ClippingShader : public Shader {
public:
    ClippingShader();

    // Shader uniforms
    void setClipRect(const glm::vec2& position, const glm::vec2& size);
    void setStencilMask(uint32_t mask);
    void setAlphaThreshold(float threshold);
    void setInverted(bool inverted);

private:
    static const std::string vertexSource;
    static const std::string fragmentSource;
};

} // namespace ui
} // namespace void_contingency
```

### 4. Create Clipping Tests

Create `tests/unit/ui/clip/ClipTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/clip/ClipManager.hpp"
#include "ui/components/Mask.hpp"

using namespace void_contingency::ui;

TEST(ClipManagerTest, ClipRectManagement) {
    auto& manager = ClipManager::getInstance();

    // Create test clip rect
    ClipRect rect;
    rect.position = glm::vec2(10.0f, 10.0f);
    rect.size = glm::vec2(100.0f, 100.0f);

    // Push clip rect
    manager.pushClipRect(rect);
    EXPECT_EQ(manager.getClipRects().size(), 1);
    EXPECT_EQ(manager.getClipRects()[0].position, rect.position);

    // Pop clip rect
    manager.popClipRect();
    EXPECT_EQ(manager.getClipRects().size(), 0);
}

TEST(ClipManagerTest, ScissorTesting) {
    auto& manager = ClipManager::getInstance();

    // Enable scissor test
    manager.enableScissorTest();
    EXPECT_TRUE(manager.isScissorEnabled());

    // Set scissor rect
    glm::vec2 position(10.0f, 10.0f);
    glm::vec2 size(100.0f, 100.0f);
    manager.setScissorRect(position, size);

    // Disable scissor test
    manager.disableScissorTest();
    EXPECT_FALSE(manager.isScissorEnabled());
}

TEST(MaskTest, MaskProperties) {
    auto mask = std::make_shared<Mask>();

    // Set mask properties
    mask->setMaskType(MaskType::Alpha);
    mask->setInverted(true);
    mask->setAlphaThreshold(0.5f);

    // Verify properties
    EXPECT_EQ(mask->getMaskType(), MaskType::Alpha);
    EXPECT_TRUE(mask->isInverted());
    EXPECT_EQ(mask->getAlphaThreshold(), 0.5f);
}

TEST(MaskTest, MaskRendering) {
    auto mask = std::make_shared<Mask>();

    // Setup mask
    mask->setupMask();

    // Render mask
    mask->render();

    // Cleanup mask
    mask->cleanupMask();
}
```

## Next Steps

1. Add performance optimizations
2. Implement animation curves
3. Add more effect types
4. Implement nested clipping

## Common Pitfalls

1. **Scissor Testing**: Handle coordinate transformations
2. **Stencil Masking**: Manage stencil buffer state
3. **Alpha Masking**: Handle transparency
4. **Performance**: Optimize clipping operations

## Best Practices

1. Use appropriate clipping methods
2. Handle nested clipping properly
3. Optimize clipping operations
4. Support different mask types
5. Implement proper cleanup

## Additional Resources

- [Theme System Tutorial](../theme/theme_system.md)
- [Nested Clipping Tutorial](./nested_clipping.md)
- [Clipping Optimizations Tutorial](./clipping_optimizations.md)
- [Clipping Debugging Tutorial](./clipping_debugging.md)
- [Clipping Serialization Tutorial](./clipping_serialization.md)
- [Clipping Compression Tutorial](./clipping_compression.md)
