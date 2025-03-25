# Implementing Post-Processing Effects

## Navigation

- Previous: [Component Animations Tutorial](./component_animations.md)
- Next: [Theme System Tutorial](../theme/theme_system.md)

This tutorial will guide you through implementing post-processing effects for "Void Contingency", providing visual enhancements and effects.

## Overview

The post-processing system will:

- Support multiple effect types
- Enable effect chaining
- Handle effect parameters
- Provide performance optimizations
- Support effect blending

## Implementation Steps

### 1. Create Effect System

Create `include/ui/effects/Effect.hpp`:

```cpp
#pragma once

#include "ui/Component.hpp"
#include <string>
#include <memory>
#include <glm/glm.hpp>

namespace void_contingency {
namespace ui {

enum class EffectType {
    Blur,
    Shadow,
    Glow,
    ColorAdjust,
    Outline
};

struct EffectParameters {
    float intensity{1.0f};
    float radius{1.0f};
    glm::vec4 color{1.0f};
    float opacity{1.0f};
    bool enabled{true};
};

class Effect {
public:
    using EffectPtr = std::shared_ptr<Effect>;

    Effect(EffectType type);
    virtual ~Effect() = default;

    // Effect properties
    void setParameters(const EffectParameters& params) { parameters_ = params; }
    const EffectParameters& getParameters() const { return parameters_; }

    void setEnabled(bool enabled) { parameters_.enabled = enabled; }
    bool isEnabled() const { return parameters_.enabled; }

    // Effect interface
    virtual void apply(ComponentPtr component) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void cleanup() = 0;

protected:
    EffectType type_;
    EffectParameters parameters_;
};

} // namespace ui
} // namespace void_contingency
```

### 2. Create Blur Effect

Create `include/ui/effects/BlurEffect.hpp`:

```cpp
#pragma once

#include "ui/effects/Effect.hpp"
#include "ui/render/Shader.hpp"
#include "ui/render/Texture.hpp"

namespace void_contingency {
namespace ui {

class BlurEffect : public Effect {
public:
    BlurEffect();

    // Blur specific
    void setRadius(float radius) { parameters_.radius = radius; }
    void setIntensity(float intensity) { parameters_.intensity = intensity; }

    // Effect interface
    void apply(ComponentPtr component) override;
    void update(float deltaTime) override;
    void cleanup() override;

private:
    std::shared_ptr<Shader> blurShader_;
    std::shared_ptr<Texture> tempTexture_;

    void setupShaders();
    void setupTextures(const glm::vec2& size);
    void applyHorizontalBlur();
    void applyVerticalBlur();
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create Shadow Effect

Create `include/ui/effects/ShadowEffect.hpp`:

```cpp
#pragma once

#include "ui/effects/Effect.hpp"
#include "ui/render/Shader.hpp"
#include "ui/render/Texture.hpp"

namespace void_contingency {
namespace ui {

class ShadowEffect : public Effect {
public:
    ShadowEffect();

    // Shadow specific
    void setOffset(const glm::vec2& offset) { offset_ = offset; }
    void setColor(const glm::vec4& color) { parameters_.color = color; }
    void setBlurRadius(float radius) { parameters_.radius = radius; }

    // Effect interface
    void apply(ComponentPtr component) override;
    void update(float deltaTime) override;
    void cleanup() override;

private:
    std::shared_ptr<Shader> shadowShader_;
    std::shared_ptr<Texture> shadowTexture_;
    glm::vec2 offset_{0.0f, 2.0f};

    void setupShaders();
    void setupTextures(const glm::vec2& size);
    void renderShadow();
};

} // namespace ui
} // namespace void_contingency
```

### 4. Create Effect Manager

Create `include/ui/effects/EffectManager.hpp`:

```cpp
#pragma once

#include "ui/effects/Effect.hpp"
#include <vector>
#include <memory>

namespace void_contingency {
namespace ui {

class EffectManager {
public:
    static EffectManager& getInstance();

    // Effect management
    void addEffect(ComponentPtr component, EffectPtr effect);
    void removeEffect(ComponentPtr component, EffectPtr effect);
    void update(float deltaTime);
    void clear();

    // Effect control
    void enableEffect(ComponentPtr component, EffectType type);
    void disableEffect(ComponentPtr component, EffectType type);
    void setEffectParameters(ComponentPtr component, EffectType type,
                           const EffectParameters& params);

private:
    EffectManager() = default;
    ~EffectManager() = default;
    EffectManager(const EffectManager&) = delete;
    EffectManager& operator=(const EffectManager&) = delete;

    struct ComponentEffects {
        ComponentPtr component;
        std::vector<EffectPtr> effects;
    };
    std::vector<ComponentEffects> componentEffects_;
};

} // namespace ui
} // namespace void_contingency
```

### 5. Create Effect Tests

Create `tests/unit/ui/effects/EffectTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/effects/Effect.hpp"
#include "ui/effects/BlurEffect.hpp"
#include "ui/effects/ShadowEffect.hpp"
#include "ui/effects/EffectManager.hpp"
#include "ui/components/Button.hpp"

using namespace void_contingency::ui;

TEST(BlurEffectTest, EffectApplication) {
    auto button = std::make_shared<Button>("Test");
    auto blurEffect = std::make_shared<BlurEffect>();

    // Set blur parameters
    EffectParameters params;
    params.radius = 2.0f;
    params.intensity = 0.5f;
    blurEffect->setParameters(params);

    // Apply effect
    blurEffect->apply(button);

    // Verify parameters
    EXPECT_EQ(blurEffect->getParameters().radius, 2.0f);
    EXPECT_EQ(blurEffect->getParameters().intensity, 0.5f);
}

TEST(ShadowEffectTest, EffectApplication) {
    auto button = std::make_shared<Button>("Test");
    auto shadowEffect = std::make_shared<ShadowEffect>();

    // Set shadow parameters
    EffectParameters params;
    params.color = glm::vec4(0.0f, 0.0f, 0.0f, 0.5f);
    params.radius = 4.0f;
    shadowEffect->setParameters(params);
    shadowEffect->setOffset(glm::vec2(2.0f, 2.0f));

    // Apply effect
    shadowEffect->apply(button);

    // Verify parameters
    EXPECT_EQ(shadowEffect->getParameters().color.a, 0.5f);
    EXPECT_EQ(shadowEffect->getParameters().radius, 4.0f);
}

TEST(EffectManagerTest, EffectManagement) {
    auto& manager = EffectManager::getInstance();
    auto button = std::make_shared<Button>("Test");
    auto blurEffect = std::make_shared<BlurEffect>();

    // Add effect
    manager.addEffect(button, blurEffect);

    // Update effect
    manager.update(0.5f);

    // Set parameters
    EffectParameters params;
    params.radius = 2.0f;
    manager.setEffectParameters(button, EffectType::Blur, params);

    // Verify effect is managed
    manager.removeEffect(button, blurEffect);
    manager.update(0.5f);
}
```

## Next Steps

1. Implement clipping and masking
2. Add performance optimizations
3. Implement animation curves
4. Add more effect types

## Common Pitfalls

1. **Performance**: Handle effect rendering efficiently
2. **Memory Management**: Manage texture resources
3. **Effect Chaining**: Handle effect dependencies
4. **State Management**: Track effect states

## Best Practices

1. Use appropriate effect parameters
2. Implement efficient rendering
3. Handle effect cleanup properly
4. Support effect chaining
5. Optimize resource usage

## Additional Resources

- [Component Animations Tutorial](./component_animations.md)
- [Theme System Tutorial](../theme/theme_system.md)
- [Clipping and Masking Tutorial](../clipping/clipping_masking.md)
- [Nested Clipping Tutorial](../clipping/nested_clipping.md)
- [Clipping Optimizations Tutorial](../clipping/clipping_optimizations.md)
- [Clipping Debugging Tutorial](../clipping/clipping_debugging.md)
- [Clipping Serialization Tutorial](../clipping/clipping_serialization.md)
- [Clipping Compression Tutorial](../clipping/clipping_compression.md)
