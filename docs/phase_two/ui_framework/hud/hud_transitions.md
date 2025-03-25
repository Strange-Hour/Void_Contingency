# Implementing HUD Transitions

## Navigation

- Previous: [HUD State Management Tutorial](./hud_state_management.md)
- Next: [HUD Input Handling Tutorial](./hud_input_handling.md)

This tutorial will guide you through implementing a transition system for the HUD in "Void Contingency", focusing on smooth state changes, animations, and visual effects.

## Overview

The HUD transition system will:

- Enable smooth transitions between states
- Support multiple transition types
- Handle transition timing and easing
- Provide visual feedback during transitions
- Support transition queuing and sequencing
- Enable transition customization

## Implementation Steps

### 1. Create HUD Transition Types

Create `include/ui/hud/HUDTransition.hpp`:

```cpp
#pragma once

#include "ui/hud/HUDTypes.hpp"
#include "ui/hud/HUDState.hpp"
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <chrono>

namespace void_contingency {
namespace ui {

enum class TransitionType {
    None,
    Fade,
    Slide,
    Scale,
    Rotate,
    Color,
    Blur,
    Glow,
    Sequence
};

enum class TransitionDirection {
    None,
    Left,
    Right,
    Up,
    Down,
    In,
    Out
};

struct TransitionOptions {
    float duration{0.5f};
    float delay{0.0f};
    EasingType easing{EasingType::Linear};
    TransitionDirection direction{TransitionDirection::None};
    bool loop{false};
    int repeatCount{1};
    bool reverse{false};
    bool autoReverse{false};
    float blurAmount{0.0f};
    float glowIntensity{0.0f};
};

struct TransitionState {
    float progress{0.0f};
    float currentValue{0.0f};
    bool isPlaying{false};
    bool isPaused{false};
    bool isComplete{false};
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point pauseTime;
};

struct HUDTransition {
    std::string id;
    TransitionType type;
    TransitionOptions options;
    TransitionState state;
    HUDState fromState;
    HUDState toState;
    std::function<void(float)> updateCallback;
    std::function<void()> completeCallback;
};

using TransitionCallback = std::function<void(const HUDTransition&)>;

} // namespace ui
} // namespace void_contingency
```

### 2. Create HUD Transition Manager

Create `include/ui/hud/HUDTransitionManager.hpp`:

```cpp
#pragma once

#include "ui/hud/HUDTransition.hpp"
#include <memory>
#include <vector>
#include <unordered_map>

namespace void_contingency {
namespace ui {

class HUDTransitionManager {
public:
    static HUDTransitionManager& getInstance();

    // Transition management
    void addTransition(const HUDTransition& transition);
    void removeTransition(const std::string& id);
    void pauseTransition(const std::string& id);
    void resumeTransition(const std::string& id);
    void stopTransition(const std::string& id);

    // Transition control
    void update(float deltaTime);
    void playAll();
    void pauseAll();
    void stopAll();

    // Transition state
    bool isTransitionPlaying(const std::string& id) const;
    float getTransitionProgress(const std::string& id) const;
    TransitionState getTransitionState(const std::string& id) const;

    // Transition subscription
    void subscribeToTransition(const std::string& id, TransitionCallback callback);
    void unsubscribeFromTransition(const std::string& id, TransitionCallback callback);

private:
    HUDTransitionManager() = default;
    ~HUDTransitionManager() = default;
    HUDTransitionManager(const HUDTransitionManager&) = delete;
    HUDTransitionManager& operator=(const HUDTransitionManager&) = delete;

    // Transition state
    std::unordered_map<std::string, HUDTransition> transitions_;
    std::unordered_map<std::string, std::vector<TransitionCallback>> callbacks_;

    // Helper methods
    void updateTransition(HUDTransition& transition, float deltaTime);
    void notifyCallbacks(const std::string& id, const HUDTransition& transition);
    float calculateEasing(float progress, EasingType easing);
    void handleTransitionComplete(HUDTransition& transition);
    void applyTransitionEffect(HUDTransition& transition, float progress);
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create HUD Transition View

Create `include/ui/hud/HUDTransitionView.hpp`:

```cpp
#pragma once

#include "ui/hud/HUDTransition.hpp"
#include "ui/components/Component.hpp"
#include <memory>

namespace void_contingency {
namespace ui {

class HUDTransitionView : public Component {
public:
    HUDTransitionView();
    ~HUDTransitionView() override;

    // View control
    void setTransition(const HUDTransition& transition);
    void setTransitionOptions(const TransitionOptions& options);
    void setTransitionState(const TransitionState& state);

    // Interactive features
    void setInteractive(bool interactive) { interactive_ = interactive; }
    bool isInteractive() const { return interactive_; }

    // View customization
    void setShowProgress(bool show) { showProgress_ = show; }
    void setShowControls(bool show) { showControls_ = show; }
    void setShowTimeline(bool show) { showTimeline_ = show; }

protected:
    void render(Renderer& renderer) override;
    void handleInput(const InputEvent& event) override;
    void update() override;

private:
    // View state
    HUDTransition currentTransition_;
    TransitionOptions currentOptions_;
    TransitionState currentState_;
    bool interactive_{false};
    bool showProgress_{true};
    bool showControls_{true};
    bool showTimeline_{true};

    // Helper methods
    void renderProgress(Renderer& renderer);
    void renderControls(Renderer& renderer);
    void renderTimeline(Renderer& renderer);
    void handleControlInput(const InputEvent& event);
    void updateVisualization();
};

} // namespace ui
} // namespace void_contingency
```

### 4. Create Transition Tests

Create `tests/unit/ui/hud/TransitionTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/hud/HUDTransition.hpp"
#include "ui/hud/HUDTransitionManager.hpp"
#include "ui/hud/HUDTransitionView.hpp"

using namespace void_contingency::ui;

TEST(HUDTransitionManagerTest, TransitionManagement) {
    auto& manager = HUDTransitionManager::getInstance();

    // Create test transition
    HUDTransition transition;
    transition.id = "test_transition";
    transition.type = TransitionType::Fade;
    transition.options.duration = 1.0f;
    transition.options.easing = EasingType::EaseInOut;
    transition.updateCallback = [](float value) {
        // Test callback
    };

    // Add transition
    manager.addTransition(transition);

    // Verify transition
    EXPECT_TRUE(manager.isTransitionPlaying("test_transition"));
    EXPECT_EQ(manager.getTransitionProgress("test_transition"), 0.0f);

    // Update transition
    manager.update(0.5f);
    EXPECT_GT(manager.getTransitionProgress("test_transition"), 0.0f);

    // Stop transition
    manager.stopTransition("test_transition");
    EXPECT_FALSE(manager.isTransitionPlaying("test_transition"));
}

TEST(HUDTransitionViewTest, ViewRendering) {
    HUDTransitionView view;

    // Set test transition
    HUDTransition transition;
    transition.id = "test_transition";
    transition.type = TransitionType::Fade;
    transition.options.duration = 1.0f;
    view.setTransition(transition);

    // Set test state
    TransitionState state;
    state.progress = 0.5f;
    state.isPlaying = true;
    view.setTransitionState(state);

    // Verify view state
    EXPECT_FALSE(view.isInteractive());
    view.setInteractive(true);
    EXPECT_TRUE(view.isInteractive());
}
```

## Next Steps

1. Implement HUD input handling
2. Add HUD styling
3. Implement HUD persistence
4. Add HUD debugging tools

## Common Pitfalls

1. **Performance**: Optimize transition updates for many elements
2. **Timing**: Handle transition timing and synchronization
3. **State Management**: Manage transition state changes
4. **Resource Usage**: Control memory usage for transitions

## Best Practices

1. Use efficient transition updates
2. Implement proper easing
3. Support transition customization
4. Enable transition queuing
5. Provide clear feedback

## Additional Resources

- [HUD State Management Tutorial](./hud_state_management.md)
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
