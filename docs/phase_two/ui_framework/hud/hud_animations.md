# Implementing HUD Animations

## Navigation

- Previous: [HUD Elements Tutorial](./hud_elements.md)
- Next: [HUD State Management Tutorial](./hud_state_management.md)

This tutorial will guide you through implementing HUD animations for "Void Contingency", providing smooth and engaging visual feedback for game events and state changes.

## Overview

The HUD animation system will:

- Enable smooth transitions between states
- Support multiple animation types
- Handle animation timing and easing
- Provide visual feedback for state changes
- Support animation queuing and sequencing
- Enable animation customization

## Implementation Steps

### 1. Create HUD Animation Types

Create `include/ui/hud/HUDAnimation.hpp`:

```cpp
#pragma once

#include "ui/hud/HUDTypes.hpp"
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <chrono>

namespace void_contingency {
namespace ui {

enum class AnimationType {
    None,
    Fade,
    Slide,
    Scale,
    Rotate,
    Color,
    Pulse,
    Shake,
    Sequence
};

enum class EasingType {
    Linear,
    EaseIn,
    EaseOut,
    EaseInOut,
    Bounce,
    Elastic
};

struct AnimationOptions {
    float duration{0.5f};
    float delay{0.0f};
    EasingType easing{EasingType::Linear};
    bool loop{false};
    int repeatCount{1};
    bool reverse{false};
    bool autoReverse{false};
};

struct AnimationState {
    float progress{0.0f};
    float currentValue{0.0f};
    bool isPlaying{false};
    bool isPaused{false};
    bool isComplete{false};
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point pauseTime;
};

struct Animation {
    std::string id;
    AnimationType type;
    AnimationOptions options;
    AnimationState state;
    std::function<void(float)> updateCallback;
    std::function<void()> completeCallback;
};

using AnimationCallback = std::function<void(const Animation&)>;

} // namespace ui
} // namespace void_contingency
```

### 2. Create HUD Animation Manager

Create `include/ui/hud/HUDAnimationManager.hpp`:

```cpp
#pragma once

#include "ui/hud/HUDAnimation.hpp"
#include <memory>
#include <vector>
#include <unordered_map>

namespace void_contingency {
namespace ui {

class HUDAnimationManager {
public:
    static HUDAnimationManager& getInstance();

    // Animation management
    void addAnimation(const Animation& animation);
    void removeAnimation(const std::string& id);
    void pauseAnimation(const std::string& id);
    void resumeAnimation(const std::string& id);
    void stopAnimation(const std::string& id);

    // Animation control
    void update(float deltaTime);
    void playAll();
    void pauseAll();
    void stopAll();

    // Animation state
    bool isAnimationPlaying(const std::string& id) const;
    float getAnimationProgress(const std::string& id) const;
    AnimationState getAnimationState(const std::string& id) const;

    // Animation subscription
    void subscribeToAnimation(const std::string& id, AnimationCallback callback);
    void unsubscribeFromAnimation(const std::string& id, AnimationCallback callback);

private:
    HUDAnimationManager() = default;
    ~HUDAnimationManager() = default;
    HUDAnimationManager(const HUDAnimationManager&) = delete;
    HUDAnimationManager& operator=(const HUDAnimationManager&) = delete;

    // Animation state
    std::unordered_map<std::string, Animation> animations_;
    std::unordered_map<std::string, std::vector<AnimationCallback>> callbacks_;

    // Helper methods
    void updateAnimation(Animation& animation, float deltaTime);
    void notifyCallbacks(const std::string& id, const Animation& animation);
    float calculateEasing(float progress, EasingType easing);
    void handleAnimationComplete(Animation& animation);
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create HUD Animation View

Create `include/ui/hud/HUDAnimationView.hpp`:

```cpp
#pragma once

#include "ui/hud/HUDAnimation.hpp"
#include "ui/components/Component.hpp"
#include <memory>

namespace void_contingency {
namespace ui {

class HUDAnimationView : public Component {
public:
    HUDAnimationView();
    ~HUDAnimationView() override;

    // View control
    void setAnimation(const Animation& animation);
    void setAnimationOptions(const AnimationOptions& options);
    void setAnimationState(const AnimationState& state);

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
    Animation currentAnimation_;
    AnimationOptions currentOptions_;
    AnimationState currentState_;
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

### 4. Create Animation Tests

Create `tests/unit/ui/hud/AnimationTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/hud/HUDAnimation.hpp"
#include "ui/hud/HUDAnimationManager.hpp"
#include "ui/hud/HUDAnimationView.hpp"

using namespace void_contingency::ui;

TEST(HUDAnimationManagerTest, AnimationManagement) {
    auto& manager = HUDAnimationManager::getInstance();

    // Create test animation
    Animation animation;
    animation.id = "test_animation";
    animation.type = AnimationType::Fade;
    animation.options.duration = 1.0f;
    animation.options.easing = EasingType::EaseInOut;
    animation.updateCallback = [](float value) {
        // Test callback
    };

    // Add animation
    manager.addAnimation(animation);

    // Verify animation
    EXPECT_TRUE(manager.isAnimationPlaying("test_animation"));
    EXPECT_EQ(manager.getAnimationProgress("test_animation"), 0.0f);

    // Update animation
    manager.update(0.5f);
    EXPECT_GT(manager.getAnimationProgress("test_animation"), 0.0f);

    // Stop animation
    manager.stopAnimation("test_animation");
    EXPECT_FALSE(manager.isAnimationPlaying("test_animation"));
}

TEST(HUDAnimationViewTest, ViewRendering) {
    HUDAnimationView view;

    // Set test animation
    Animation animation;
    animation.id = "test_animation";
    animation.type = AnimationType::Fade;
    animation.options.duration = 1.0f;
    view.setAnimation(animation);

    // Set test state
    AnimationState state;
    state.progress = 0.5f;
    state.isPlaying = true;
    view.setAnimationState(state);

    // Verify view state
    EXPECT_FALSE(view.isInteractive());
    view.setInteractive(true);
    EXPECT_TRUE(view.isInteractive());
}
```

## Next Steps

1. Implement HUD state management
2. Add HUD transitions
3. Implement HUD input handling
4. Add HUD styling

## Common Pitfalls

1. **Performance**: Optimize animation updates for many elements
2. **Timing**: Handle animation timing and synchronization
3. **State Management**: Manage animation state transitions
4. **Resource Usage**: Control memory usage for animations

## Best Practices

1. Use efficient animation updates
2. Implement proper easing
3. Support animation customization
4. Enable animation queuing
5. Provide clear feedback

## Additional Resources

- [HUD Elements Tutorial](./hud_elements.md)
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
