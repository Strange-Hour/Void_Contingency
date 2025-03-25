# Implementing Theme System

## Navigation

- Previous: [UI Component System Tutorial](../core/ui_component_system.md)
- Next: [Menu System Tutorial](../menu/menu_system.md)

This tutorial will guide you through implementing the theme system for "Void Contingency", providing a flexible and consistent visual styling system.

## Overview

The theme system will:

- Manage visual styles
- Handle color schemes
- Process typography
- Track theme states
- Support different themes
- Implement theme transitions

## Implementation Steps

### 1. Create Theme Manager

Create `include/ui/theme/ThemeManager.hpp`:

```cpp
#pragma once

#include "ui/theme/Theme.hpp"
#include "ui/theme/ThemeStyle.hpp"
#include <vector>
#include <memory>
#include <unordered_map>

namespace void_contingency {
namespace ui {

struct ThemeEvent {
    enum class Type {
        Changed,
        Updated,
        Reset,
        Failed
    };

    Type type;
    std::string themeId;
    std::string reason;
};

class ThemeManager {
public:
    static ThemeManager& getInstance();

    // Theme state
    void initializeTheme(const std::string& themeId);
    void resetTheme(const std::string& themeId);
    bool isThemeInitialized(const std::string& themeId) const;
    const Theme& getTheme(const std::string& themeId) const;

    // Theme actions
    bool canApplyTheme(const std::string& themeId) const;
    bool applyTheme(const std::string& themeId);
    bool updateTheme(const std::string& themeId, const ThemeStyle& style);
    bool resetToDefaultTheme();

    // Theme tracking
    void recordThemeEvent(const ThemeEvent& event);
    std::vector<ThemeEvent> getThemeHistory() const;
    std::vector<std::string> getAvailableThemes() const;
    std::string getCurrentTheme() const;

    // Theme styles
    bool hasStyle(const std::string& themeId, const std::string& styleId) const;
    const ThemeStyle& getStyle(const std::string& themeId, const std::string& styleId) const;
    void addStyle(const std::string& themeId, const std::string& styleId, const ThemeStyle& style);
    void removeStyle(const std::string& themeId, const std::string& styleId);

private:
    ThemeManager() = default;
    ~ThemeManager() = default;
    ThemeManager(const ThemeManager&) = delete;
    ThemeManager& operator=(const ThemeManager&) = delete;

    // Theme state
    std::unordered_map<std::string, Theme> themes_;
    std::vector<ThemeEvent> themeHistory_;
    std::string currentTheme_;
    Theme defaultTheme_;

    // Helper methods
    void updateThemeState(const std::string& themeId);
    void processThemeChanges(const std::string& themeId);
    void validateTheme(const std::string& themeId);
};

} // namespace ui
} // namespace void_contingency
```

### 2. Create Theme Types

Create `include/ui/theme/Theme.hpp`:

```cpp
#pragma once

#include "ui/theme/ThemeStyle.hpp"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace void_contingency {
namespace ui {

struct ColorScheme {
    struct Color {
        float r, g, b, a;
    };

    Color primary;
    Color secondary;
    Color accent;
    Color background;
    Color text;
    Color error;
    Color warning;
    Color success;
};

struct Typography {
    struct Font {
        std::string family;
        float size;
        float weight;
        bool italic;
    };

    Font heading;
    Font body;
    Font button;
    Font caption;
};

class Theme {
public:
    using ThemePtr = std::shared_ptr<Theme>;

    Theme(const std::string& id, const std::string& name);

    // Theme properties
    const std::string& getId() const { return id_; }
    const std::string& getName() const { return name_; }
    const ColorScheme& getColorScheme() const { return colorScheme_; }
    const Typography& getTypography() const { return typography_; }

    // Theme styles
    void addStyle(const std::string& styleId, const ThemeStyle& style);
    void removeStyle(const std::string& styleId);
    const ThemeStyle& getStyle(const std::string& styleId) const;
    bool hasStyle(const std::string& styleId) const;

    // Theme functionality
    void updateColorScheme(const ColorScheme& scheme);
    void updateTypography(const Typography& typography);
    void resetToDefaults();

private:
    std::string id_;
    std::string name_;
    ColorScheme colorScheme_;
    Typography typography_;
    std::unordered_map<std::string, ThemeStyle> styles_;
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create Theme Style

Create `include/ui/theme/ThemeStyle.hpp`:

```cpp
#pragma once

#include <string>
#include <vector>
#include <memory>

namespace void_contingency {
namespace ui {

struct StyleProperty {
    enum class Type {
        Color,
        Size,
        Spacing,
        Border,
        Shadow,
        Animation
    };

    Type type;
    std::string name;
    std::string value;
};

class ThemeStyle {
public:
    using ThemeStylePtr = std::shared_ptr<ThemeStyle>;

    ThemeStyle(const std::string& id, const std::string& name);

    // Style properties
    const std::string& getId() const { return id_; }
    const std::string& getName() const { return name_; }
    const std::vector<StyleProperty>& getProperties() const { return properties_; }

    // Style management
    void addProperty(const StyleProperty& property);
    void removeProperty(const std::string& name);
    const StyleProperty& getProperty(const std::string& name) const;
    bool hasProperty(const std::string& name) const;

    // Style functionality
    void updateProperty(const std::string& name, const std::string& value);
    void resetToDefaults();

private:
    std::string id_;
    std::string name_;
    std::vector<StyleProperty> properties_;
};

} // namespace ui
} // namespace void_contingency
```

### 4. Create Theme Tests

Create `tests/unit/ui/theme/ThemeTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/theme/ThemeManager.hpp"
#include "ui/theme/Theme.hpp"
#include "ui/theme/ThemeStyle.hpp"

using namespace void_contingency::ui;

TEST(ThemeManagerTest, ThemeManagement) {
    auto& manager = ThemeManager::getInstance();
    auto theme = std::make_shared<Theme>("dark", "Dark Theme");

    // Initialize theme
    manager.initializeTheme("dark");
    EXPECT_TRUE(manager.isThemeInitialized("dark"));

    // Apply theme
    EXPECT_TRUE(manager.canApplyTheme("dark"));
    EXPECT_TRUE(manager.applyTheme("dark"));
    EXPECT_EQ(manager.getCurrentTheme(), "dark");

    // Check theme history
    auto history = manager.getThemeHistory();
    EXPECT_EQ(history.size(), 2); // Initialize and Apply events
}

TEST(ThemeTest, StyleManagement) {
    auto theme = std::make_shared<Theme>("dark", "Dark Theme");
    auto style = std::make_shared<ThemeStyle>("button", "Button Style");

    // Add style
    StyleProperty colorProp{StyleProperty::Type::Color, "background", "#000000"};
    style->addProperty(colorProp);
    theme->addStyle("button", *style);

    // Check style
    EXPECT_TRUE(theme->hasStyle("button"));
    const auto& buttonStyle = theme->getStyle("button");
    EXPECT_TRUE(buttonStyle.hasProperty("background"));
}

TEST(ThemeStyleTest, PropertyManagement) {
    auto style = std::make_shared<ThemeStyle>("card", "Card Style");

    // Add properties
    StyleProperty sizeProp{StyleProperty::Type::Size, "width", "100px"};
    StyleProperty borderProp{StyleProperty::Type::Border, "radius", "8px"};
    style->addProperty(sizeProp);
    style->addProperty(borderProp);

    // Check properties
    EXPECT_TRUE(style->hasProperty("width"));
    EXPECT_TRUE(style->hasProperty("radius"));
    const auto& widthProp = style->getProperty("width");
    EXPECT_EQ(widthProp.value, "100px");
}
```

## Next Steps

1. Implement menu system
2. Add theme persistence
3. Create theme analytics
4. Implement theme UI

## Common Pitfalls

1. **Consistency**: Ensure theme styles are consistent
2. **Performance**: Optimize theme changes
3. **Complexity**: Manage theme system complexity
4. **Feedback**: Provide clear theme feedback

## Best Practices

1. Design clear theme structure
2. Maintain consistent styling
3. Create meaningful theme variations
4. Implement proper theme transitions
5. Support different theme types

## Additional Resources

- [UI Component System Tutorial](../core/ui_component_system.md)
- [Menu System Tutorial](../menu/menu_system.md)
