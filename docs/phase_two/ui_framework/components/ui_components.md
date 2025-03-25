# Implementing UI Components

This tutorial will guide you through implementing specific UI components for "Void Contingency", building on top of the UI component system, layout management, and styling system.

## Overview

We'll implement the following components:

- Button: Interactive button with states and callbacks
- Text: Text rendering with formatting
- Image: Image display with scaling and tiling
- Panel: Container component with background

## Implementation Steps

### 1. Create Button Component

Create `include/ui/components/Button.hpp`:

```cpp
#pragma once

#include "ui/Component.hpp"
#include "ui/style/Style.hpp"
#include <functional>
#include <string>

namespace void_contingency {
namespace ui {

enum class ButtonState {
    Normal,
    Hover,
    Pressed,
    Disabled
};

class Button : public Component {
public:
    using ButtonPtr = std::shared_ptr<Button>;
    using ClickCallback = std::function<void()>;

    Button(const std::string& text = "");
    virtual ~Button() = default;

    // Button properties
    void setText(const std::string& text);
    const std::string& getText() const { return text_; }

    void setEnabled(bool enabled);
    bool isEnabled() const { return enabled_; }

    void setClickCallback(ClickCallback callback);
    void setHoverCallback(std::function<void(bool)> callback);

    // Component interface
    void update(float deltaTime) override;
    void render() override;
    void handleInput(const InputEvent& event) override;

protected:
    // Button state
    std::string text_;
    bool enabled_{true};
    ButtonState state_{ButtonState::Normal};
    bool isHovered_{false};
    bool isPressed_{false};

    // Callbacks
    ClickCallback clickCallback_;
    std::function<void(bool)> hoverCallback_;

    // Style properties
    void updateStyle() override;
    void applyStateStyle(ButtonState state);
};

} // namespace ui
} // namespace void_contingency
```

### 2. Create Text Component

Create `include/ui/components/Text.hpp`:

```cpp
#pragma once

#include "ui/Component.hpp"
#include "ui/style/Style.hpp"
#include <string>

namespace void_contingency {
namespace ui {

enum class TextAlignment {
    Left,
    Center,
    Right
};

class Text : public Component {
public:
    using TextPtr = std::shared_ptr<Text>;

    Text(const std::string& text = "");
    virtual ~Text() = default;

    // Text properties
    void setText(const std::string& text);
    const std::string& getText() const { return text_; }

    void setFontSize(float size);
    float getFontSize() const { return fontSize_; }

    void setAlignment(TextAlignment alignment);
    TextAlignment getAlignment() const { return alignment_; }

    void setColor(const glm::vec4& color);
    const glm::vec4& getColor() const { return color_; }

    // Component interface
    void update(float deltaTime) override;
    void render() override;
    void handleInput(const InputEvent& event) override;

protected:
    // Text properties
    std::string text_;
    float fontSize_{16.0f};
    TextAlignment alignment_{TextAlignment::Left};
    glm::vec4 color_{1.0f};

    // Style properties
    void updateStyle() override;
    void updateTextMetrics();
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create Image Component

Create `include/ui/components/Image.hpp`:

```cpp
#pragma once

#include "ui/Component.hpp"
#include "ui/style/Style.hpp"
#include <string>

namespace void_contingency {
namespace ui {

enum class ImageScaleMode {
    Stretch,
    Fit,
    Fill,
    Tile
};

class Image : public Component {
public:
    using ImagePtr = std::shared_ptr<Image>;

    Image(const std::string& imagePath = "");
    virtual ~Image() = default;

    // Image properties
    void setImage(const std::string& path);
    const std::string& getImagePath() const { return imagePath_; }

    void setScaleMode(ImageScaleMode mode);
    ImageScaleMode getScaleMode() const { return scaleMode_; }

    void setTiling(const glm::vec2& tiling);
    const glm::vec2& getTiling() const { return tiling_; }

    void setColor(const glm::vec4& color);
    const glm::vec4& getColor() const { return color_; }

    // Component interface
    void update(float deltaTime) override;
    void render() override;
    void handleInput(const InputEvent& event) override;

protected:
    // Image properties
    std::string imagePath_;
    ImageScaleMode scaleMode_{ImageScaleMode::Stretch};
    glm::vec2 tiling_{1.0f};
    glm::vec4 color_{1.0f};

    // Style properties
    void updateStyle() override;
    void updateImageMetrics();
};

} // namespace ui
} // namespace void_contingency
```

### 4. Create Panel Component

Create `include/ui/components/Panel.hpp`:

```cpp
#pragma once

#include "ui/Component.hpp"
#include "ui/style/Style.hpp"
#include <vector>

namespace void_contingency {
namespace ui {

class Panel : public Component {
public:
    using PanelPtr = std::shared_ptr<Panel>;

    Panel();
    virtual ~Panel() = default;

    // Panel properties
    void setBackgroundColor(const glm::vec4& color);
    const glm::vec4& getBackgroundColor() const { return backgroundColor_; }

    void setBorderColor(const glm::vec4& color);
    const glm::vec4& getBorderColor() const { return borderColor_; }

    void setBorderWidth(float width);
    float getBorderWidth() const { return borderWidth_; }

    void setCornerRadius(float radius);
    float getCornerRadius() const { return cornerRadius_; }

    // Component interface
    void update(float deltaTime) override;
    void render() override;
    void handleInput(const InputEvent& event) override;

protected:
    // Panel properties
    glm::vec4 backgroundColor_{0.0f, 0.0f, 0.0f, 0.0f};
    glm::vec4 borderColor_{1.0f, 1.0f, 1.0f, 1.0f};
    float borderWidth_{0.0f};
    float cornerRadius_{0.0f};

    // Style properties
    void updateStyle() override;
    void updatePanelMetrics();
};

} // namespace ui
} // namespace void_contingency
```

### 5. Create Component Tests

Create `tests/unit/ui/components/ComponentTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/components/Button.hpp"
#include "ui/components/Text.hpp"
#include "ui/components/Image.hpp"
#include "ui/components/Panel.hpp"

using namespace void_contingency::ui;

TEST(ButtonTest, ButtonCreation) {
    auto button = std::make_shared<Button>("Test Button");

    EXPECT_EQ(button->getText(), "Test Button");
    EXPECT_TRUE(button->isEnabled());
    EXPECT_EQ(button->getState(), ButtonState::Normal);
}

TEST(ButtonTest, ButtonInteraction) {
    auto button = std::make_shared<Button>("Test Button");
    bool clicked = false;

    button->setClickCallback([&clicked]() { clicked = true; });

    // Simulate hover
    InputEvent hoverEvent{InputEventType::MouseMove, glm::vec2(10.0f, 10.0f)};
    button->handleInput(hoverEvent);
    EXPECT_EQ(button->getState(), ButtonState::Hover);

    // Simulate click
    InputEvent clickEvent{InputEventType::MouseButtonDown, glm::vec2(10.0f, 10.0f)};
    button->handleInput(clickEvent);
    EXPECT_TRUE(clicked);
}

TEST(TextTest, TextProperties) {
    auto text = std::make_shared<Text>("Test Text");

    text->setFontSize(24.0f);
    text->setAlignment(TextAlignment::Center);
    text->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

    EXPECT_EQ(text->getFontSize(), 24.0f);
    EXPECT_EQ(text->getAlignment(), TextAlignment::Center);
    EXPECT_EQ(text->getColor().r, 1.0f);
}

TEST(ImageTest, ImageProperties) {
    auto image = std::make_shared<Image>("test.png");

    image->setScaleMode(ImageScaleMode::Fit);
    image->setTiling(glm::vec2(2.0f, 2.0f));
    image->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

    EXPECT_EQ(image->getScaleMode(), ImageScaleMode::Fit);
    EXPECT_EQ(image->getTiling().x, 2.0f);
    EXPECT_EQ(image->getColor().r, 1.0f);
}

TEST(PanelTest, PanelProperties) {
    auto panel = std::make_shared<Panel>();

    panel->setBackgroundColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    panel->setBorderColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    panel->setBorderWidth(2.0f);
    panel->setCornerRadius(4.0f);

    EXPECT_EQ(panel->getBackgroundColor().a, 1.0f);
    EXPECT_EQ(panel->getBorderColor().r, 1.0f);
    EXPECT_EQ(panel->getBorderWidth(), 2.0f);
    EXPECT_EQ(panel->getCornerRadius(), 4.0f);
}
```

## Next Steps

1. Implement component rendering
2. Add component animations
3. Implement component event handling
4. Add component layout support

## Common Pitfalls

1. **State Management**: Handle component state transitions
2. **Event Propagation**: Manage event bubbling
3. **Style Updates**: Handle style changes efficiently
4. **Performance**: Optimize component rendering

## Best Practices

1. Use component composition
2. Implement proper event handling
3. Support style customization
4. Optimize component updates
5. Handle component lifecycle

## Additional Resources

- [UI Component System Tutorial](../core/ui_component_system.md)
- [Layout Management Tutorial](../core/layout_management.md)
- [Styling System Tutorial](../core/styling_system.md)
