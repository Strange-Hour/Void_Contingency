# Implementing Menu Debugging

## Navigation

- Previous: [Menu Persistence Tutorial](./menu_persistence.md)
- Next: [Menu Analytics Tutorial](./menu_analytics.md)

This tutorial will guide you through implementing menu debugging for "Void Contingency", providing a robust system for debugging menu components, states, and interactions.

## Overview

The menu debugging system will:

- Support debug logging
- Enable state visualization
- Provide debugging tools
- Handle debug events
- Enable debug customization
- Support debug analytics

## Implementation Steps

### 1. Create Menu Debugging Types

Create `include/ui/menu/MenuDebugging.hpp`:

```cpp
#pragma once

#include "ui/menu/MenuComponents.hpp"
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include <variant>

namespace void_contingency {
namespace ui {

enum class DebugLevel {
    None,
    Error,
    Warning,
    Info,
    Debug,
    Verbose,
    Custom
};

enum class DebugCategory {
    General,
    State,
    Input,
    Rendering,
    Performance,
    Memory,
    Network,
    Custom
};

enum class DebugMode {
    Off,
    Basic,
    Advanced,
    Expert,
    Custom
};

struct DebugOptions {
    DebugLevel level{DebugLevel::None};
    DebugCategory category{DebugCategory::General};
    DebugMode mode{DebugMode::Off};
    bool enableLogging{false};
    bool enableVisualization{false};
    bool enableTools{false};
    bool enableEvents{false};
    bool enableAnalytics{false};
    std::string path;
    std::string filename;
    std::string extension;
    std::unordered_map<std::string, std::variant<bool, int, float, std::string>> properties;
};

struct DebugEvent {
    std::string id;
    DebugLevel level{DebugLevel::None};
    DebugCategory category{DebugCategory::General};
    std::string message;
    std::string timestamp;
    std::string source;
    std::vector<std::string> tags;
    std::unordered_map<std::string, std::variant<bool, int, float, std::string>> data;
};

class MenuDebugging {
public:
    MenuDebugging();
    ~MenuDebugging();

    // Debugging identification
    void setId(const std::string& id) { properties_.id = id; }
    const std::string& getId() const { return properties_.id; }

    // Debugging properties
    void setLevel(DebugLevel level) { properties_.level = level; }
    DebugLevel getLevel() const { return properties_.level; }

    void setCategory(DebugCategory category) { properties_.category = category; }
    DebugCategory getCategory() const { return properties_.category; }

    void setMode(DebugMode mode) { properties_.mode = mode; }
    DebugMode getMode() const { return properties_.mode; }

    // Debugging control
    void enable();
    void disable();
    void reset();
    void clear();

    // Debugging events
    void log(const DebugEvent& event);
    void visualize(const DebugEvent& event);
    void handle(const DebugEvent& event);
    void analyze(const DebugEvent& event);

    // Debugging tools
    void showTools();
    void hideTools();
    void toggleTools();
    void updateTools();

    // Debugging events
    void setOnLog(std::function<void(const DebugEvent&)> callback) { onLog_ = callback; }
    void setOnVisualize(std::function<void(const DebugEvent&)> callback) { onVisualize_ = callback; }
    void setOnHandle(std::function<void(const DebugEvent&)> callback) { onHandle_ = callback; }
    void setOnAnalyze(std::function<void(const DebugEvent&)> callback) { onAnalyze_ = callback; }

    // Debugging update
    void update(float deltaTime);

private:
    struct DebuggingProperties {
        std::string id;
        DebugLevel level{DebugLevel::None};
        DebugCategory category{DebugCategory::General};
        DebugMode mode{DebugMode::Off};
        bool enableLogging{false};
        bool enableVisualization{false};
        bool enableTools{false};
        bool enableEvents{false};
        bool enableAnalytics{false};
        std::string path;
        std::string filename;
        std::string extension;
        std::unordered_map<std::string, std::variant<bool, int, float, std::string>> properties;
    };

    DebuggingProperties properties_;
    std::vector<DebugEvent> eventQueue_;
    bool isEnabled_{false};
    bool isActive_{false};
    bool showTools_{false};

    // Debugging callbacks
    std::function<void(const DebugEvent&)> onLog_;
    std::function<void(const DebugEvent&)> onVisualize_;
    std::function<void(const DebugEvent&)> onHandle_;
    std::function<void(const DebugEvent&)> onAnalyze_;

    // Helper methods
    void processEvent(const DebugEvent& event);
    void handleVisualization(const DebugEvent& event);
    void updateTools();
    void validateEvents();
    void backupEvents();
    void restoreEvents();
};

} // namespace ui
} // namespace void_contingency
```

### 2. Create Menu Debugging Manager

Create `include/ui/menu/MenuDebuggingManager.hpp`:

```cpp
#pragma once

#include "ui/menu/MenuDebugging.hpp"
#include <memory>
#include <vector>
#include <unordered_map>
#include <queue>
#include <functional>

namespace void_contingency {
namespace ui {

class MenuDebuggingManager {
public:
    static MenuDebuggingManager& getInstance();

    // Debugging management
    void addDebugging(std::shared_ptr<MenuDebugging> debugging);
    void removeDebugging(const std::string& id);
    void updateDebugging(const std::string& id, std::shared_ptr<MenuDebugging> debugging);
    std::shared_ptr<MenuDebugging> getDebugging(const std::string& id) const;
    std::vector<std::shared_ptr<MenuDebugging>> getAllDebugging() const;

    // Debugging control
    void enableDebugging(const std::string& id);
    void disableDebugging(const std::string& id);
    void resetDebugging(const std::string& id);
    void clearDebugging(const std::string& id);

    // Debugging events
    void logEvent(const std::string& id, const DebugEvent& event);
    void visualizeEvent(const std::string& id, const DebugEvent& event);
    void handleEvent(const std::string& id, const DebugEvent& event);
    void analyzeEvent(const std::string& id, const DebugEvent& event);

    // Debugging tools
    void showTools(const std::string& id);
    void hideTools(const std::string& id);
    void toggleTools(const std::string& id);
    void updateTools(const std::string& id);

    // Debugging events
    void setOnDebugEvent(std::function<void(const std::string&, const DebugEvent&)> callback);
    void setOnVisualizationEvent(std::function<void(const std::string&, const DebugEvent&)> callback);
    void setOnToolEvent(std::function<void(const std::string&, const DebugEvent&)> callback);

    // Debugging analytics
    void enableAnalytics(bool enable);
    void setAnalyticsLevel(int level);
    void logAnalytics(const std::string& message);

    // Debugging cleanup
    void cleanup();
    void reset();
    void clear();

private:
    MenuDebuggingManager() = default;
    ~MenuDebuggingManager() = default;
    MenuDebuggingManager(const MenuDebuggingManager&) = delete;
    MenuDebuggingManager& operator=(const MenuDebuggingManager&) = delete;

    // Debugging storage
    std::unordered_map<std::string, std::shared_ptr<MenuDebugging>> debugging_;
    std::queue<DebugEvent> eventQueue_;
    std::vector<std::string> activeDebugging_;
    bool analyticsEnabled_{false};
    int analyticsLevel_{0};

    // Debugging callbacks
    std::function<void(const std::string&, const DebugEvent&)> onDebugEvent_;
    std::function<void(const std::string&, const DebugEvent&)> onVisualizationEvent_;
    std::function<void(const std::string&, const DebugEvent&)> onToolEvent_;

    // Helper methods
    void updateDebugging(float deltaTime);
    void handleDebuggingEvents();
    void processEventQueue();
    void validateDebugging();
    void cleanupDebuggingResources();
    void backupDebuggingState();
    void restoreDebuggingState();
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create Menu Debugging View

Create `include/ui/menu/MenuDebuggingView.hpp`:

```cpp
#pragma once

#include "ui/menu/MenuDebugging.hpp"
#include "ui/components/Component.hpp"
#include <memory>

namespace void_contingency {
namespace ui {

class MenuDebuggingView : public Component {
public:
    MenuDebuggingView();
    ~MenuDebuggingView() override;

    // View control
    void setDebugging(std::shared_ptr<MenuDebugging> debugging);
    void setDebugging(const std::vector<std::shared_ptr<MenuDebugging>>& debugging);

    // Interactive features
    void setInteractive(bool interactive) { interactive_ = interactive; }
    bool isInteractive() const { return interactive_; }

    // View customization
    void setShowEvents(bool show) { showEvents_ = show; }
    void setShowVisualization(bool show) { showVisualization_ = show; }
    void setShowTools(bool show) { showTools_ = show; }
    void setShowControls(bool show) { showControls_ = show; }

protected:
    void render(Renderer& renderer) override;
    void handleInput(const InputEvent& event) override;
    void update() override;

private:
    // View state
    std::shared_ptr<MenuDebugging> debugging_;
    std::vector<std::shared_ptr<MenuDebugging>> debugging_;
    bool interactive_{false};
    bool showEvents_{true};
    bool showVisualization_{true};
    bool showTools_{true};
    bool showControls_{true};

    // Helper methods
    void renderEvents(Renderer& renderer);
    void renderVisualization(Renderer& renderer);
    void renderTools(Renderer& renderer);
    void renderControls(Renderer& renderer);
    void handleDebuggingInput(const InputEvent& event);
    void updateVisualization();
};

} // namespace ui
} // namespace void_contingency
```

### 4. Create Menu Debugging Tests

Create `tests/unit/ui/menu/DebuggingTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/menu/MenuDebugging.hpp"
#include "ui/menu/MenuDebuggingManager.hpp"
#include "ui/menu/MenuDebuggingView.hpp"

using namespace void_contingency::ui;

TEST(MenuDebuggingTest, DebuggingCreation) {
    auto debugging = std::make_shared<MenuDebugging>();
    debugging->setId("test_debugging");
    debugging->setLevel(DebugLevel::Info);
    debugging->setCategory(DebugCategory::State);

    EXPECT_EQ(debugging->getId(), "test_debugging");
    EXPECT_EQ(debugging->getLevel(), DebugLevel::Info);
    EXPECT_EQ(debugging->getCategory(), DebugCategory::State);
}

TEST(MenuDebuggingTest, DebuggingEvents) {
    auto debugging = std::make_shared<MenuDebugging>();
    debugging->setId("test_debugging");

    DebugEvent event;
    event.id = "test_event";
    event.level = DebugLevel::Info;
    event.category = DebugCategory::State;
    event.message = "Test message";
    event.timestamp = "2024-01-01T00:00:00Z";
    event.source = "test_source";

    debugging->log(event);

    // Note: In a real test, you would verify the event was logged correctly
    // This might involve checking a log file or event queue
}

TEST(MenuDebuggingManagerTest, DebuggingManagement) {
    auto& manager = MenuDebuggingManager::getInstance();

    // Create test debugging
    auto debugging = std::make_shared<MenuDebugging>();
    debugging->setId("test_debugging");
    debugging->setLevel(DebugLevel::Info);

    // Add debugging
    manager.addDebugging(debugging);

    // Verify debugging
    auto retrievedDebugging = manager.getDebugging("test_debugging");
    EXPECT_EQ(retrievedDebugging->getId(), "test_debugging");
    EXPECT_EQ(retrievedDebugging->getLevel(), DebugLevel::Info);
}

TEST(MenuDebuggingViewTest, ViewRendering) {
    MenuDebuggingView view;

    // Create test debugging
    auto debugging = std::make_shared<MenuDebugging>();
    debugging->setId("test_debugging");
    debugging->setLevel(DebugLevel::Info);

    // Set debugging
    view.setDebugging(debugging);

    // Verify view state
    EXPECT_FALSE(view.isInteractive());
    view.setInteractive(true);
    EXPECT_TRUE(view.isInteractive());
}
```

## Next Steps

1. Implement menu analytics
2. Add menu optimization
3. Implement menu sharing
4. Add menu testing

## Common Pitfalls

1. **Event Handling**: Handle debug events correctly
2. **Event Visualization**: Manage event visualization properly
3. **Event Tools**: Handle debug tools correctly
4. **Event State**: Manage event state properly

## Best Practices

1. Use proper debug levels
2. Implement event handling
3. Handle event visualization
4. Manage event state
5. Support customization

## Additional Resources

- [Menu Components Tutorial](./menu_components.md)
- [Menu Navigation Tutorial](./menu_navigation.md)
- [Menu State Management Tutorial](./menu_state_management.md)
- [Menu Transitions Tutorial](./menu_transitions.md)
- [Menu Input Handling Tutorial](./menu_input_handling.md)
- [Menu Persistence Tutorial](./menu_persistence.md)
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
- [Menu Analytics Tutorial](./menu_analytics.md)
- [Menu Optimization Tutorial](./menu_optimization.md)
- [Menu Sharing Tutorial](./menu_sharing.md)
- [Menu Testing Tutorial](./menu_testing.md)
