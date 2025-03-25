# Implementing Menu Transitions

## Navigation

- Previous: [Menu Navigation Tutorial](./menu_navigation.md)
- Next: [Menu Input Handling Tutorial](./menu_input_handling.md)

This tutorial will guide you through implementing menu transitions for "Void Contingency", providing a robust system for animating menu state changes and transitions.

## Overview

The menu transitions system will:

- Support various transition types
- Handle transition timing
- Manage transition effects
- Enable transition customization
- Support transition chaining
- Provide transition debugging

## Implementation Steps

### 1. Create Menu Transition Types

Create `include/ui/menu/MenuTransition.hpp`:

```cpp
#pragma once

#include "ui/menu/MenuComponents.hpp"
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <variant>

namespace void_contingency {
namespace ui {

enum class TransitionType {
    Fade,
    Slide,
    Scale,
    Rotate,
    Flip,
    Morph,
    Shake,
    Bounce,
    Elastic,
    Custom
};

enum class TransitionDirection {
    In,
    Out,
    Left,
    Right,
    Up,
    Down,
    Forward,
    Backward,
    Clockwise,
    CounterClockwise
};

enum class TransitionTiming {
    Linear,
    EaseIn,
    EaseOut,
    EaseInOut,
    Bounce,
    Elastic,
    Custom
};

struct TransitionProperties {
    TransitionType type{TransitionType::Fade};
    TransitionDirection direction{TransitionDirection::In};
    TransitionTiming timing{TransitionTiming::Linear};
    float duration{0.5f};
    float delay{0.0f};
    float repeat{0};
    bool reverse{false};
    bool alternate{false};
    bool yoyo{false};
    bool autoReverse{false};
    bool autoPlay{false};
    bool loop{false};
    bool paused{false};
    bool completed{false};
    float progress{0.0f};
    float speed{1.0f};
    float easing{0.0f};
    std::string customEasing;
    std::vector<std::string> targets;
    std::unordered_map<std::string, std::variant<bool, int, float, std::string>> properties;
};

class MenuTransition {
public:
    MenuTransition();
    ~MenuTransition();

    // Transition identification
    void setId(const std::string& id) { properties_.id = id; }
    const std::string& getId() const { return properties_.id; }

    // Transition properties
    void setType(TransitionType type) { properties_.type = type; }
    TransitionType getType() const { return properties_.type; }

    void setDirection(TransitionDirection direction) { properties_.direction = direction; }
    TransitionDirection getDirection() const { return properties_.direction; }

    void setTiming(TransitionTiming timing) { properties_.timing = timing; }
    TransitionTiming getTiming() const { return properties_.timing; }

    void setDuration(float duration) { properties_.duration = duration; }
    float getDuration() const { return properties_.duration; }

    void setDelay(float delay) { properties_.delay = delay; }
    float getDelay() const { return properties_.delay; }

    void setRepeat(float repeat) { properties_.repeat = repeat; }
    float getRepeat() const { return properties_.repeat; }

    // Transition control
    void play();
    void pause();
    void resume();
    void stop();
    void reset();
    void reverse();
    void complete();

    // Transition state
    bool isPlaying() const;
    bool isPaused() const;
    bool isCompleted() const;
    float getProgress() const;
    float getSpeed() const;

    // Transition targets
    void addTarget(const std::string& target);
    void removeTarget(const std::string& target);
    const std::vector<std::string>& getTargets() const;

    // Transition events
    void setOnStart(std::function<void()> callback) { onStart_ = callback; }
    void setOnUpdate(std::function<void(float)> callback) { onUpdate_ = callback; }
    void setOnComplete(std::function<void()> callback) { onComplete_ = callback; }
    void setOnReverse(std::function<void()> callback) { onReverse_ = callback; }
    void setOnRepeat(std::function<void()> callback) { onRepeat_ = callback; }

    // Transition update
    void update(float deltaTime);

private:
    TransitionProperties properties_;
    float currentTime_{0.0f};
    float startTime_{0.0f};
    float endTime_{0.0f};
    bool isPlaying_{false};
    bool isPaused_{false};
    bool isCompleted_{false};

    // Transition callbacks
    std::function<void()> onStart_;
    std::function<void(float)> onUpdate_;
    std::function<void()> onComplete_;
    std::function<void()> onReverse_;
    std::function<void()> onRepeat_;

    // Helper methods
    void updateProgress(float deltaTime);
    void updateProperties();
    void handleEvents();
    void validateProperties();
    void backupProperties();
    void restoreProperties();
};

} // namespace ui
} // namespace void_contingency
```

### 2. Create Menu Transition Manager

Create `include/ui/menu/MenuTransitionManager.hpp`:

```cpp
#pragma once

#include "ui/menu/MenuTransition.hpp"
#include <memory>
#include <vector>
#include <unordered_map>
#include <queue>
#include <functional>

namespace void_contingency {
namespace ui {

class MenuTransitionManager {
public:
    static MenuTransitionManager& getInstance();

    // Transition management
    void addTransition(std::shared_ptr<MenuTransition> transition);
    void removeTransition(const std::string& id);
    void updateTransition(const std::string& id, std::shared_ptr<MenuTransition> transition);
    std::shared_ptr<MenuTransition> getTransition(const std::string& id) const;
    std::vector<std::shared_ptr<MenuTransition>> getAllTransitions() const;

    // Transition control
    void playTransition(const std::string& id);
    void pauseTransition(const std::string& id);
    void resumeTransition(const std::string& id);
    void stopTransition(const std::string& id);
    void completeTransition(const std::string& id);

    // Transition chaining
    void chainTransitions(const std::vector<std::string>& ids);
    void parallelTransitions(const std::vector<std::string>& ids);
    void sequenceTransitions(const std::vector<std::string>& ids);

    // Transition events
    void setOnTransitionStart(std::function<void(const std::string&)> callback);
    void setOnTransitionComplete(std::function<void(const std::string&)> callback);
    void setOnTransitionChainComplete(std::function<void()> callback);

    // Transition debugging
    void enableDebugging(bool enable);
    void setDebugLevel(int level);
    void logTransition(const std::string& message);

    // Transition cleanup
    void cleanup();
    void reset();
    void clear();

private:
    MenuTransitionManager() = default;
    ~MenuTransitionManager() = default;
    MenuTransitionManager(const MenuTransitionManager&) = delete;
    MenuTransitionManager& operator=(const MenuTransitionManager&) = delete;

    // Transition storage
    std::unordered_map<std::string, std::shared_ptr<MenuTransition>> transitions_;
    std::queue<std::string> transitionQueue_;
    std::vector<std::string> activeTransitions_;
    bool debuggingEnabled_{false};
    int debugLevel_{0};

    // Transition callbacks
    std::function<void(const std::string&)> onTransitionStart_;
    std::function<void(const std::string&)> onTransitionComplete_;
    std::function<void()> onTransitionChainComplete_;

    // Helper methods
    void updateTransitions(float deltaTime);
    void handleTransitionEvents();
    void processTransitionQueue();
    void validateTransitions();
    void cleanupTransitionResources();
    void backupTransitionState();
    void restoreTransitionState();
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create Menu Transition View

Create `include/ui/menu/MenuTransitionView.hpp`:

```cpp
#pragma once

#include "ui/menu/MenuTransition.hpp"
#include "ui/components/Component.hpp"
#include <memory>

namespace void_contingency {
namespace ui {

class MenuTransitionView : public Component {
public:
    MenuTransitionView();
    ~MenuTransitionView() override;

    // View control
    void setTransition(std::shared_ptr<MenuTransition> transition);
    void setTransitions(const std::vector<std::shared_ptr<MenuTransition>>& transitions);

    // Interactive features
    void setInteractive(bool interactive) { interactive_ = interactive; }
    bool isInteractive() const { return interactive_; }

    // View customization
    void setShowProperties(bool show) { showProperties_ = show; }
    void setShowTimeline(bool show) { showTimeline_ = show; }
    void setShowPreview(bool show) { showPreview_ = show; }
    void setShowControls(bool show) { showControls_ = show; }

protected:
    void render(Renderer& renderer) override;
    void handleInput(const InputEvent& event) override;
    void update() override;

private:
    // View state
    std::shared_ptr<MenuTransition> transition_;
    std::vector<std::shared_ptr<MenuTransition>> transitions_;
    bool interactive_{false};
    bool showProperties_{true};
    bool showTimeline_{true};
    bool showPreview_{true};
    bool showControls_{true};

    // Helper methods
    void renderProperties(Renderer& renderer);
    void renderTimeline(Renderer& renderer);
    void renderPreview(Renderer& renderer);
    void renderControls(Renderer& renderer);
    void handleTransitionInput(const InputEvent& event);
    void updateVisualization();
};

} // namespace ui
} // namespace void_contingency
```

### 4. Create Menu Transition Tests

Create `tests/unit/ui/menu/TransitionTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/menu/MenuTransition.hpp"
#include "ui/menu/MenuTransitionManager.hpp"
#include "ui/menu/MenuTransitionView.hpp"

using namespace void_contingency::ui;

TEST(MenuTransitionTest, TransitionCreation) {
    auto transition = std::make_shared<MenuTransition>();
    transition->setId("test_transition");
    transition->setType(TransitionType::Fade);
    transition->setDuration(0.5f);

    EXPECT_EQ(transition->getId(), "test_transition");
    EXPECT_EQ(transition->getType(), TransitionType::Fade);
    EXPECT_EQ(transition->getDuration(), 0.5f);
}

TEST(MenuTransitionTest, TransitionControl) {
    auto transition = std::make_shared<MenuTransition>();
    transition->setId("test_transition");

    transition->play();
    EXPECT_TRUE(transition->isPlaying());

    transition->pause();
    EXPECT_TRUE(transition->isPaused());

    transition->resume();
    EXPECT_TRUE(transition->isPlaying());

    transition->stop();
    EXPECT_FALSE(transition->isPlaying());
}

TEST(MenuTransitionTest, TransitionTargets) {
    auto transition = std::make_shared<MenuTransition>();
    transition->setId("test_transition");

    transition->addTarget("target1");
    transition->addTarget("target2");

    const auto& targets = transition->getTargets();
    EXPECT_EQ(targets.size(), 2);
    EXPECT_EQ(targets[0], "target1");
    EXPECT_EQ(targets[1], "target2");
}

TEST(MenuTransitionManagerTest, TransitionManagement) {
    auto& manager = MenuTransitionManager::getInstance();

    // Create test transition
    auto transition = std::make_shared<MenuTransition>();
    transition->setId("test_transition");
    transition->setType(TransitionType::Fade);

    // Add transition
    manager.addTransition(transition);

    // Verify transition
    auto retrievedTransition = manager.getTransition("test_transition");
    EXPECT_EQ(retrievedTransition->getId(), "test_transition");
    EXPECT_EQ(retrievedTransition->getType(), TransitionType::Fade);
}

TEST(MenuTransitionViewTest, ViewRendering) {
    MenuTransitionView view;

    // Create test transition
    auto transition = std::make_shared<MenuTransition>();
    transition->setId("test_transition");
    transition->setType(TransitionType::Fade);

    // Set transition
    view.setTransition(transition);

    // Verify view state
    EXPECT_FALSE(view.isInteractive());
    view.setInteractive(true);
    EXPECT_TRUE(view.isInteractive());
}
```

## Next Steps

1. Implement menu input handling
2. Add menu persistence
3. Implement menu debugging
4. Add menu analytics

## Common Pitfalls

1. **Transition Timing**: Handle transition timing correctly
2. **Transition Effects**: Apply effects properly
3. **Transition Chaining**: Manage transition chains correctly
4. **Transition State**: Handle transition state properly

## Best Practices

1. Use proper transition types
2. Implement transition timing
3. Handle transition events
4. Manage transition state
5. Support customization

## Additional Resources

- [Menu Navigation Tutorial](./menu_navigation.md)
- [Menu Input Handling Tutorial](./menu_input_handling.md)
- [Menu Persistence Tutorial](./menu_persistence.md)
- [Menu Debugging Tutorial](./menu_debugging.md)
- [Menu Analytics Tutorial](./menu_analytics.md)
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
