# Implementing Styling System

## Navigation

- Previous: [Layout Management Tutorial](./layout_management.md)
- Next: [Component Rendering Tutorial](./component_rendering.md)

This tutorial will guide you through implementing the styling system for "Void Contingency", which handles the visual appearance and theming of UI components.

## Overview

The styling system will:

- Support style properties and inheritance
- Handle theme management
- Support style animations and transitions
- Provide style validation
- Enable style debugging
- Support style caching

## Implementation Steps

### 1. Create Style Properties

Create `include/ui/style/Style.hpp`:

```cpp
#pragma once

#include <string>
#include <variant>
#include <unordered_map>
#include <memory>
#include <functional>
#include <glm/glm.hpp>

namespace void_contingency {
namespace ui {

enum class StylePropertyType {
    Color,
    Float,
    Int,
    Bool,
    String,
    Vector2,
    Vector4
};

struct StyleValue {
    std::variant<glm::vec4, float, int, bool, std::string, glm::vec2> value;
    StylePropertyType type;
};

class StyleProperty {
public:
    StyleProperty(const std::string& name, StylePropertyType type);

    const std::string& getName() const { return name_; }
    StylePropertyType getType() const { return type_; }

    template<typename T>
    T getValue() const {
        return std::get<T>(value_.value);
    }

    template<typename T>
    void setValue(const T& value) {
        value_.value = value;
    }

private:
    std::string name_;
    StylePropertyType type_;
    StyleValue value_;
};

class Style {
public:
    using StylePtr = std::shared_ptr<Style>;
    using PropertyMap = std::unordered_map<std::string, StyleProperty>;

    Style(const std::string& name = "");
    virtual ~Style() = default;

    // Style properties
    void setProperty(const std::string& name, const StyleValue& value);
    StyleValue getProperty(const std::string& name) const;
    bool hasProperty(const std::string& name) const;
    void removeProperty(const std::string& name);

    // Style inheritance
    void setParent(StylePtr parent);
    StylePtr getParent() const { return parent_; }
    void addChild(StylePtr child);
    void removeChild(StylePtr child);

    // Style animation
    void animateProperty(const std::string& name, const StyleValue& target,
                        float duration, std::function<float(float)> easing = nullptr);
    void stopAnimation(const std::string& name);
    bool isAnimating(const std::string& name) const;

    // Style validation
    bool validateProperty(const std::string& name, const StyleValue& value) const;
    bool validateStyle() const;

    // Style debugging
    void setDebugMode(bool debug) { debugMode_ = debug; }
    bool isDebugMode() const { return debugMode_; }
    void dumpStyle() const;

protected:
    // Style properties
    std::string name_;
    PropertyMap properties_;
    StylePtr parent_;
    std::vector<StylePtr> children_;
    bool debugMode_{false};

    // Animation state
    struct Animation {
        StyleValue startValue;
        StyleValue targetValue;
        float duration;
        float elapsed;
        std::function<float(float)> easing;
    };
    std::unordered_map<std::string, Animation> animations_;

    // Helper methods
    void updateAnimations(float deltaTime);
    void applyInheritance();
    void validateInheritance() const;
};

} // namespace ui
} // namespace void_contingency
```

### 2. Create Theme Manager

Create `include/ui/style/ThemeManager.hpp`:

```cpp
#pragma once

#include "ui/style/Style.hpp"
#include <unordered_map>
#include <memory>

namespace void_contingency {
namespace ui {

class ThemeManager {
public:
    static ThemeManager& getInstance();

    // Theme management
    void loadTheme(const std::string& name, const std::string& filepath);
    void setActiveTheme(const std::string& name);
    StylePtr getTheme(const std::string& name) const;
    StylePtr getActiveTheme() const { return activeTheme_; }

    // Style resolution
    StyleValue resolveStyleValue(const std::string& componentType,
                                const std::string& propertyName) const;
    StylePtr resolveStyle(const std::string& componentType) const;

    // Theme switching
    void switchTheme(const std::string& name, float duration = 0.0f);
    bool isThemeTransitioning() const { return transitioning_; }

private:
    ThemeManager() = default;
    ~ThemeManager() = default;
    ThemeManager(const ThemeManager&) = delete;
    ThemeManager& operator=(const ThemeManager&) = delete;

    // Theme storage
    std::unordered_map<std::string, StylePtr> themes_;
    StylePtr activeTheme_;
    bool transitioning_{false};

    // Theme transition
    struct ThemeTransition {
        StylePtr fromTheme;
        StylePtr toTheme;
        float duration;
        float elapsed;
    } transition_;
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create Style Animator

Create `include/ui/style/StyleAnimator.hpp`:

```cpp
#pragma once

#include "ui/style/Style.hpp"
#include <functional>
#include <queue>

namespace void_contingency {
namespace ui {

class StyleAnimator {
public:
    static StyleAnimator& getInstance();

    // Animation management
    void animate(StylePtr style, const std::string& propertyName,
                const StyleValue& targetValue, float duration,
                std::function<float(float)> easing = nullptr);
    void stopAnimation(StylePtr style, const std::string& propertyName);
    void stopAllAnimations(StylePtr style);

    // Animation update
    void update(float deltaTime);

private:
    StyleAnimator() = default;
    ~StyleAnimator() = default;
    StyleAnimator(const StyleAnimator&) = delete;
    StyleAnimator& operator=(const StyleAnimator&) = delete;

    // Animation queue
    struct AnimationTask {
        StylePtr style;
        std::string propertyName;
        StyleValue startValue;
        StyleValue targetValue;
        float duration;
        float elapsed;
        std::function<float(float)> easing;
    };
    std::queue<AnimationTask> animationQueue_;
};

} // namespace ui
} // namespace void_contingency
```

### 4. Create Style Tests

Create `tests/unit/ui/style/StyleTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/style/Style.hpp"
#include "ui/style/ThemeManager.hpp"
#include "ui/style/StyleAnimator.hpp"

using namespace void_contingency::ui;

TEST(StyleTest, StyleProperties) {
    auto style = std::make_shared<Style>("test");

    // Set color property
    style->setProperty("backgroundColor", StyleValue{glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), StylePropertyType::Color});

    // Set float property
    style->setProperty("opacity", StyleValue{0.5f, StylePropertyType::Float});

    // Set string property
    style->setProperty("fontFamily", StyleValue{std::string("Arial"), StylePropertyType::String});

    // Verify properties
    EXPECT_TRUE(style->hasProperty("backgroundColor"));
    EXPECT_TRUE(style->hasProperty("opacity"));
    EXPECT_TRUE(style->hasProperty("fontFamily"));

    auto bgColor = style->getProperty("backgroundColor");
    EXPECT_EQ(std::get<glm::vec4>(bgColor.value).r, 1.0f);

    auto opacity = style->getProperty("opacity");
    EXPECT_EQ(std::get<float>(opacity.value), 0.5f);

    auto fontFamily = style->getProperty("fontFamily");
    EXPECT_EQ(std::get<std::string>(fontFamily.value), "Arial");
}

TEST(StyleTest, StyleInheritance) {
    auto parentStyle = std::make_shared<Style>("parent");
    auto childStyle = std::make_shared<Style>("child");

    // Set parent property
    parentStyle->setProperty("backgroundColor", StyleValue{glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), StylePropertyType::Color});

    // Set child property
    childStyle->setProperty("opacity", StyleValue{0.5f, StylePropertyType::Float});

    // Set inheritance
    childStyle->setParent(parentStyle);

    // Verify inheritance
    auto bgColor = childStyle->getProperty("backgroundColor");
    EXPECT_EQ(std::get<glm::vec4>(bgColor.value).r, 1.0f);

    auto opacity = childStyle->getProperty("opacity");
    EXPECT_EQ(std::get<float>(opacity.value), 0.5f);
}

TEST(StyleTest, StyleAnimation) {
    auto style = std::make_shared<Style>("test");
    auto& animator = StyleAnimator::getInstance();

    // Set initial value
    style->setProperty("opacity", StyleValue{0.0f, StylePropertyType::Float});

    // Animate property
    animator.animate(style, "opacity", StyleValue{1.0f, StylePropertyType::Float}, 1.0f);

    // Update animation
    animator.update(0.5f);

    // Verify intermediate value
    auto opacity = style->getProperty("opacity");
    EXPECT_GT(std::get<float>(opacity.value), 0.0f);
    EXPECT_LT(std::get<float>(opacity.value), 1.0f);

    // Complete animation
    animator.update(0.5f);

    // Verify final value
    opacity = style->getProperty("opacity");
    EXPECT_EQ(std::get<float>(opacity.value), 1.0f);
}

TEST(StyleTest, ThemeManagement) {
    auto& themeManager = ThemeManager::getInstance();

    // Create themes
    auto lightTheme = std::make_shared<Style>("light");
    lightTheme->setProperty("backgroundColor", StyleValue{glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), StylePropertyType::Color});

    auto darkTheme = std::make_shared<Style>("dark");
    darkTheme->setProperty("backgroundColor", StyleValue{glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), StylePropertyType::Color});

    // Load themes
    themeManager.loadTheme("light", "themes/light.json");
    themeManager.loadTheme("dark", "themes/dark.json");

    // Switch themes
    themeManager.switchTheme("dark");

    // Verify theme switch
    auto activeTheme = themeManager.getActiveTheme();
    auto bgColor = activeTheme->getProperty("backgroundColor");
    EXPECT_EQ(std::get<glm::vec4>(bgColor.value).r, 0.0f);
}
```

## Next Steps

1. Implement specific UI components (Button, Text, Image, etc.)
2. Add style validation rules
3. Implement style debugging tools
4. Add style performance optimizations

## Common Pitfalls

1. **Style Inheritance**: Handle circular dependencies
2. **Animation**: Handle interrupted animations
3. **Theme Switching**: Handle missing properties
4. **Performance**: Optimize style resolution

## Best Practices

1. Use style inheritance effectively
2. Implement proper animation handling
3. Validate style properties
4. Support style debugging
5. Optimize style performance

## Additional Resources

- [Layout Management Tutorial](./layout_management.md)
- [Component Rendering Tutorial](./component_rendering.md)
- [Component Animations Tutorial](../components/component_animations.md)
- [Post-Processing Effects Tutorial](../components/post_processing.md)
- [Theme System Tutorial](../themes/theme_system.md)
