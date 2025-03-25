# Implementing Layout Management

## Navigation

- Previous: [UI Component System Tutorial](./ui_component_system.md)
- Next: [Styling System Tutorial](./styling_system.md)

This tutorial will guide you through implementing layout management for "Void Contingency", providing flexible and responsive UI layouts.

## Overview

The layout management system will:

- Support different layout types (Grid, Flex, Stack)
- Handle component sizing and constraints
- Manage component alignment and spacing
- Support layout updates and invalidation
- Handle layout animations and transitions
- Provide layout debugging tools

## Implementation Steps

### 1. Create Layout Types

Create `include/ui/layout/Layout.hpp`:

```cpp
#pragma once

#include "ui/Component.hpp"
#include <vector>
#include <memory>
#include <functional>

namespace void_contingency {
namespace ui {

enum class LayoutType {
    None,
    Grid,
    Flex,
    Stack
};

enum class LayoutDirection {
    Horizontal,
    Vertical
};

enum class LayoutAlignment {
    Start,
    Center,
    End,
    Stretch
};

enum class LayoutWrap {
    NoWrap,
    Wrap
};

struct LayoutConstraints {
    float minWidth{0.0f};
    float maxWidth{std::numeric_limits<float>::infinity()};
    float minHeight{0.0f};
    float maxHeight{std::numeric_limits<float>::infinity()};
    float preferredWidth{0.0f};
    float preferredHeight{0.0f};
};

class Layout {
public:
    using LayoutPtr = std::shared_ptr<Layout>;
    using ComponentPtr = std::shared_ptr<Component>;

    Layout(LayoutType type = LayoutType::None);
    virtual ~Layout() = default;

    // Layout properties
    void setType(LayoutType type) { type_ = type; }
    LayoutType getType() const { return type_; }
    void setDirection(LayoutDirection direction) { direction_ = direction; }
    LayoutDirection getDirection() const { return direction_; }
    void setAlignment(LayoutAlignment alignment) { alignment_ = alignment; }
    LayoutAlignment getAlignment() const { return alignment_; }
    void setWrap(LayoutWrap wrap) { wrap_ = wrap; }
    LayoutWrap getWrap() const { return wrap_; }
    void setSpacing(float spacing) { spacing_ = spacing; }
    float getSpacing() const { return spacing_; }
    void setPadding(float padding) { padding_ = padding; }
    float getPadding() const { return padding_; }

    // Layout constraints
    void setConstraints(const LayoutConstraints& constraints) { constraints_ = constraints; }
    const LayoutConstraints& getConstraints() const { return constraints_; }

    // Layout operations
    virtual void calculateLayout(const ComponentPtr& container);
    virtual void applyLayout(const ComponentPtr& container);
    virtual void invalidateLayout() { needsUpdate_ = true; }
    bool needsUpdate() const { return needsUpdate_; }

    // Layout debugging
    void setDebugMode(bool debug) { debugMode_ = debug; }
    bool isDebugMode() const { return debugMode_; }

protected:
    // Layout properties
    LayoutType type_{LayoutType::None};
    LayoutDirection direction_{LayoutDirection::Horizontal};
    LayoutAlignment alignment_{LayoutAlignment::Start};
    LayoutWrap wrap_{LayoutWrap::NoWrap};
    float spacing_{0.0f};
    float padding_{0.0f};
    LayoutConstraints constraints_;

    // Layout state
    bool needsUpdate_{true};
    bool debugMode_{false};

    // Helper methods
    virtual void calculateChildLayouts(const ComponentPtr& container);
    virtual void applyChildLayouts(const ComponentPtr& container);
    virtual void debugDraw(const ComponentPtr& container);
};

} // namespace ui
} // namespace void_contingency
```

### 2. Create Grid Layout

Create `include/ui/layout/GridLayout.hpp`:

```cpp
#pragma once

#include "ui/layout/Layout.hpp"

namespace void_contingency {
namespace ui {

struct GridCell {
    int row{0};
    int column{0};
    int rowSpan{1};
    int columnSpan{1};
    LayoutAlignment alignment{LayoutAlignment::Stretch};
};

class GridLayout : public Layout {
public:
    GridLayout();

    // Grid properties
    void setColumns(int columns) { columns_ = columns; }
    int getColumns() const { return columns_; }
    void setRows(int rows) { rows_ = rows; }
    int getRows() const { return rows_; }
    void setCellSpacing(float spacing) { cellSpacing_ = spacing; }
    float getCellSpacing() const { return cellSpacing_; }

    // Cell management
    void setCell(const ComponentPtr& component, const GridCell& cell);
    GridCell getCell(const ComponentPtr& component) const;

    // Layout operations
    void calculateLayout(const ComponentPtr& container) override;
    void applyLayout(const ComponentPtr& container) override;

private:
    // Grid properties
    int columns_{1};
    int rows_{1};
    float cellSpacing_{0.0f};

    // Cell tracking
    std::unordered_map<ComponentPtr, GridCell> cells_;

    // Helper methods
    void calculateCellSizes(const ComponentPtr& container,
                           std::vector<float>& columnWidths,
                           std::vector<float>& rowHeights);
    void applyCellLayouts(const ComponentPtr& container,
                         const std::vector<float>& columnWidths,
                         const std::vector<float>& rowHeights);
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create Flex Layout

Create `include/ui/layout/FlexLayout.hpp`:

```cpp
#pragma once

#include "ui/layout/Layout.hpp"

namespace void_contingency {
namespace ui {

struct FlexItem {
    float grow{0.0f};
    float shrink{1.0f};
    float basis{0.0f};
    LayoutAlignment alignment{LayoutAlignment::Stretch};
};

class FlexLayout : public Layout {
public:
    FlexLayout();

    // Flex properties
    void setGrow(float grow) { grow_ = grow; }
    float getGrow() const { return grow_; }
    void setShrink(float shrink) { shrink_ = shrink; }
    float getShrink() const { return shrink_; }
    void setBasis(float basis) { basis_ = basis; }
    float getBasis() const { return basis_; }

    // Item management
    void setItem(const ComponentPtr& component, const FlexItem& item);
    FlexItem getItem(const ComponentPtr& component) const;

    // Layout operations
    void calculateLayout(const ComponentPtr& container) override;
    void applyLayout(const ComponentPtr& container) override;

private:
    // Flex properties
    float grow_{0.0f};
    float shrink_{1.0f};
    float basis_{0.0f};

    // Item tracking
    std::unordered_map<ComponentPtr, FlexItem> items_;

    // Helper methods
    void calculateItemSizes(const ComponentPtr& container,
                           std::vector<float>& sizes);
    void applyItemLayouts(const ComponentPtr& container,
                         const std::vector<float>& sizes);
};

} // namespace ui
} // namespace void_contingency
```

### 4. Create Stack Layout

Create `include/ui/layout/StackLayout.hpp`:

```cpp
#pragma once

#include "ui/layout/Layout.hpp"

namespace void_contingency {
namespace ui {

struct StackItem {
    LayoutAlignment alignment{LayoutAlignment::Center};
    float offset{0.0f};
};

class StackLayout : public Layout {
public:
    StackLayout();

    // Stack properties
    void setOffset(float offset) { offset_ = offset; }
    float getOffset() const { return offset_; }

    // Item management
    void setItem(const ComponentPtr& component, const StackItem& item);
    StackItem getItem(const ComponentPtr& component) const;

    // Layout operations
    void calculateLayout(const ComponentPtr& container) override;
    void applyLayout(const ComponentPtr& container) override;

private:
    // Stack properties
    float offset_{0.0f};

    // Item tracking
    std::unordered_map<ComponentPtr, StackItem> items_;

    // Helper methods
    void calculateItemPositions(const ComponentPtr& container,
                               std::vector<glm::vec2>& positions);
    void applyItemLayouts(const ComponentPtr& container,
                         const std::vector<glm::vec2>& positions);
};

} // namespace ui
} // namespace void_contingency
```

### 5. Create Layout Tests

Create `tests/unit/ui/layout/LayoutTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/layout/Layout.hpp"
#include "ui/layout/GridLayout.hpp"
#include "ui/layout/FlexLayout.hpp"
#include "ui/layout/StackLayout.hpp"

using namespace void_contingency::ui;

TEST(LayoutTest, GridLayout) {
    auto container = std::make_shared<Component>("container");
    auto layout = std::make_shared<GridLayout>();
    container->setLayout(layout);

    // Set up grid
    layout->setColumns(2);
    layout->setRows(2);
    layout->setCellSpacing(10.0f);

    // Add components
    auto component1 = std::make_shared<Component>("component1");
    auto component2 = std::make_shared<Component>("component2");
    auto component3 = std::make_shared<Component>("component3");

    container->addChild(component1);
    container->addChild(component2);
    container->addChild(component3);

    // Set cell positions
    layout->setCell(component1, GridCell{0, 0, 1, 1});
    layout->setCell(component2, GridCell{0, 1, 1, 1});
    layout->setCell(component3, GridCell{1, 0, 1, 2});

    // Set container size
    container->setSize(200.0f, 200.0f);

    // Calculate and apply layout
    layout->calculateLayout(container);
    layout->applyLayout(container);

    // Verify component positions
    EXPECT_EQ(component1->getLayout().calculatedPosition.x, 0.0f);
    EXPECT_EQ(component1->getLayout().calculatedPosition.y, 0.0f);
    EXPECT_EQ(component1->getLayout().calculatedSize.x, 95.0f);
    EXPECT_EQ(component1->getLayout().calculatedSize.y, 95.0f);

    EXPECT_EQ(component2->getLayout().calculatedPosition.x, 105.0f);
    EXPECT_EQ(component2->getLayout().calculatedPosition.y, 0.0f);
    EXPECT_EQ(component2->getLayout().calculatedSize.x, 95.0f);
    EXPECT_EQ(component2->getLayout().calculatedSize.y, 95.0f);

    EXPECT_EQ(component3->getLayout().calculatedPosition.x, 0.0f);
    EXPECT_EQ(component3->getLayout().calculatedPosition.y, 105.0f);
    EXPECT_EQ(component3->getLayout().calculatedSize.x, 200.0f);
    EXPECT_EQ(component3->getLayout().calculatedSize.y, 95.0f);
}

TEST(LayoutTest, FlexLayout) {
    auto container = std::make_shared<Component>("container");
    auto layout = std::make_shared<FlexLayout>();
    container->setLayout(layout);

    // Set up flex container
    layout->setDirection(LayoutDirection::Horizontal);
    layout->setSpacing(10.0f);

    // Add components
    auto component1 = std::make_shared<Component>("component1");
    auto component2 = std::make_shared<Component>("component2");
    auto component3 = std::make_shared<Component>("component3");

    container->addChild(component1);
    container->addChild(component2);
    container->addChild(component3);

    // Set flex properties
    layout->setItem(component1, FlexItem{1.0f, 0.0f, 50.0f});
    layout->setItem(component2, FlexItem{2.0f, 0.0f, 50.0f});
    layout->setItem(component3, FlexItem{1.0f, 0.0f, 50.0f});

    // Set container size
    container->setSize(300.0f, 100.0f);

    // Calculate and apply layout
    layout->calculateLayout(container);
    layout->applyLayout(container);

    // Verify component sizes
    EXPECT_EQ(component1->getLayout().calculatedSize.x, 70.0f);
    EXPECT_EQ(component2->getLayout().calculatedSize.x, 140.0f);
    EXPECT_EQ(component3->getLayout().calculatedSize.x, 70.0f);
}

TEST(LayoutTest, StackLayout) {
    auto container = std::make_shared<Component>("container");
    auto layout = std::make_shared<StackLayout>();
    container->setLayout(layout);

    // Set up stack container
    layout->setOffset(10.0f);

    // Add components
    auto component1 = std::make_shared<Component>("component1");
    auto component2 = std::make_shared<Component>("component2");
    auto component3 = std::make_shared<Component>("component3");

    container->addChild(component1);
    container->addChild(component2);
    container->addChild(component3);

    // Set stack properties
    layout->setItem(component1, StackItem{LayoutAlignment::Center, 0.0f});
    layout->setItem(component2, StackItem{LayoutAlignment::Center, 20.0f});
    layout->setItem(component3, StackItem{LayoutAlignment::Center, 40.0f});

    // Set container size
    container->setSize(200.0f, 200.0f);

    // Calculate and apply layout
    layout->calculateLayout(container);
    layout->applyLayout(container);

    // Verify component positions
    EXPECT_EQ(component1->getLayout().calculatedPosition.y, 0.0f);
    EXPECT_EQ(component2->getLayout().calculatedPosition.y, 20.0f);
    EXPECT_EQ(component3->getLayout().calculatedPosition.y, 40.0f);
}
```

## Next Steps

1. Implement specific UI components (Button, Text, Image, etc.)
2. Add layout animations and transitions
3. Implement layout debugging tools
4. Add layout performance optimizations

## Common Pitfalls

1. **Layout Calculation**: Handle complex layout scenarios
2. **Performance**: Optimize layout updates
3. **Animation**: Handle layout transitions smoothly
4. **Constraints**: Handle conflicting layout constraints

## Best Practices

1. Use appropriate layout types
2. Implement layout caching
3. Handle layout invalidation
4. Support layout debugging
5. Optimize layout performance

## Additional Resources

- [UI Component System Tutorial](./ui_component_system.md)
- [Styling System Tutorial](./styling_system.md)
- [Component Rendering Tutorial](./component_rendering.md)
- [Component Animations Tutorial](../components/component_animations.md)
- [Post-Processing Effects Tutorial](../components/post_processing.md)
