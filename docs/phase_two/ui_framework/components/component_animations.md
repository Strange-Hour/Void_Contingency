# Implementing Component Animations

## Navigation

- Previous: [Component Rendering Tutorial](../core/component_rendering.md)
- Next: [Post-Processing Effects Tutorial](./post_processing.md)

This tutorial will guide you through implementing the animation system for UI components in "Void Contingency", focusing on smooth transitions and effects.

## Overview

The animation system will:

- Support property animations
- Handle state transitions
- Enable layout animations
- Provide effect animations
- Support animation curves and timing

## Implementation Steps

### 1. Create Animation System

Create `include/ui/animation/Animation.hpp`:

```cpp
#pragma once

#include "ui/Component.hpp"
#include <functional>
#include <variant>
#include <glm/glm.hpp>

namespace void_contingency {
namespace ui {

enum class AnimationType {
    Property,
    State,
    Layout,
    Effect
};

enum class EasingType {
    Linear,
    EaseIn,
    EaseOut,
    EaseInOut,
    Bounce,
    Elastic
};

struct AnimationValue {
    std::variant<float, glm::vec2, glm::vec4, bool> value;
    std::type_index type;
};

class Animation {
public:
    using AnimationPtr = std::shared_ptr<Animation>;
    using AnimationCallback = std::function<void()>;

    Animation(AnimationType type, float duration);
    virtual ~Animation() = default;

    // Animation control
    void start();
    void pause();
    void resume();
    void stop();
    void update(float deltaTime);

    // Animation properties
    void setEasing(EasingType easing) { easingType_ = easing; }
    void setDelay(float delay) { delay_ = delay; }
    void setLoop(bool loop) { loop_ = loop; }
    void setCallback(AnimationCallback callback) { callback_ = callback; }

    // Animation state
    bool isRunning() const { return running_; }
    bool isPaused() const { return paused_; }
    bool isFinished() const { return finished_; }
    float getProgress() const { return progress_; }

protected:
    // Animation properties
    AnimationType type_;
    float duration_;
    float elapsed_{0.0f};
    float delay_{0.0f};
    float progress_{0.0f};
    EasingType easingType_{EasingType::Linear};
    bool loop_{false};
    bool running_{false};
    bool paused_{false};
    bool finished_{false};

    // Callback
    AnimationCallback callback_;

    // Helper methods
    float applyEasing(float t) const;
    virtual void onStart() = 0;
    virtual void onUpdate(float progress) = 0;
    virtual void onFinish() = 0;
};

} // namespace ui
} // namespace void_contingency
```

### 2. Create Property Animation

Create `include/ui/animation/PropertyAnimation.hpp`:

```cpp
#pragma once

#include "ui/animation/Animation.hpp"
#include "ui/Component.hpp"
#include <string>

namespace void_contingency {
namespace ui {

class PropertyAnimation : public Animation {
public:
    PropertyAnimation(ComponentPtr component, const std::string& property,
                     const AnimationValue& startValue, const AnimationValue& endValue,
                     float duration);

    // Property animation specific
    void setStartValue(const AnimationValue& value) { startValue_ = value; }
    void setEndValue(const AnimationValue& value) { endValue_ = value; }
    void setProperty(const std::string& property) { property_ = property; }

protected:
    void onStart() override;
    void onUpdate(float progress) override;
    void onFinish() override;

private:
    ComponentPtr component_;
    std::string property_;
    AnimationValue startValue_;
    AnimationValue endValue_;
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create State Animation

Create `include/ui/animation/StateAnimation.hpp`:

```cpp
#pragma once

#include "ui/animation/Animation.hpp"
#include "ui/Component.hpp"
#include <unordered_map>

namespace void_contingency {
namespace ui {

class StateAnimation : public Animation {
public:
    StateAnimation(ComponentPtr component, float duration);

    // State animation specific
    void addStateProperty(const std::string& state, const std::string& property,
                         const AnimationValue& value);
    void setState(const std::string& state);
    void setTransition(const std::string& fromState, const std::string& toState);

protected:
    void onStart() override;
    void onUpdate(float progress) override;
    void onFinish() override;

private:
    ComponentPtr component_;
    std::string currentState_;
    std::string targetState_;
    std::unordered_map<std::string, std::unordered_map<std::string, AnimationValue>> stateProperties_;
};

} // namespace ui
} // namespace void_contingency
```

### 4. Create Animation Manager

Create `include/ui/animation/AnimationManager.hpp`:

```cpp
#pragma once

#include "ui/animation/Animation.hpp"
#include <vector>
#include <memory>

namespace void_contingency {
namespace ui {

class AnimationManager {
public:
    static AnimationManager& getInstance();

    // Animation management
    void addAnimation(AnimationPtr animation);
    void removeAnimation(AnimationPtr animation);
    void update(float deltaTime);
    void clear();

    // Animation control
    void pauseAll();
    void resumeAll();
    void stopAll();

private:
    AnimationManager() = default;
    ~AnimationManager() = default;
    AnimationManager(const AnimationManager&) = delete;
    AnimationManager& operator=(const AnimationManager&) = delete;

    std::vector<AnimationPtr> animations_;
};

} // namespace ui
} // namespace void_contingency
```

### 5. Create Animation Tests

Create `tests/unit/ui/animation/AnimationTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/animation/Animation.hpp"
#include "ui/animation/PropertyAnimation.hpp"
#include "ui/animation/StateAnimation.hpp"
#include "ui/animation/AnimationManager.hpp"
#include "ui/components/Button.hpp"

using namespace void_contingency::ui;

TEST(PropertyAnimationTest, BasicAnimation) {
    auto button = std::make_shared<Button>("Test");
    auto animation = std::make_shared<PropertyAnimation>(
        button,
        "opacity",
        AnimationValue{0.0f, std::type_index(typeid(float))},
        AnimationValue{1.0f, std::type_index(typeid(float))},
        1.0f
    );

    animation->start();
    animation->update(0.5f);
    EXPECT_GT(animation->getProgress(), 0.0f);
    EXPECT_LT(animation->getProgress(), 1.0f);

    animation->update(0.5f);
    EXPECT_EQ(animation->getProgress(), 1.0f);
    EXPECT_TRUE(animation->isFinished());
}

TEST(StateAnimationTest, StateTransition) {
    auto button = std::make_shared<Button>("Test");
    auto animation = std::make_shared<StateAnimation>(button, 1.0f);

    // Add state properties
    animation->addStateProperty("normal", "backgroundColor",
        AnimationValue{glm::vec4(1.0f), std::type_index(typeid(glm::vec4))});
    animation->addStateProperty("hover", "backgroundColor",
        AnimationValue{glm::vec4(0.8f), std::type_index(typeid(glm::vec4))});

    // Set state transition
    animation->setState("normal");
    animation->setTransition("normal", "hover");
    animation->start();

    animation->update(0.5f);
    EXPECT_GT(animation->getProgress(), 0.0f);
    EXPECT_LT(animation->getProgress(), 1.0f);
}

TEST(AnimationManagerTest, AnimationManagement) {
    auto& manager = AnimationManager::getInstance();
    auto button = std::make_shared<Button>("Test");
    auto animation = std::make_shared<PropertyAnimation>(
        button,
        "opacity",
        AnimationValue{0.0f, std::type_index(typeid(float))},
        AnimationValue{1.0f, std::type_index(typeid(float))},
        1.0f
    );

    manager.addAnimation(animation);
    manager.update(0.5f);
    EXPECT_GT(animation->getProgress(), 0.0f);

    manager.removeAnimation(animation);
    manager.update(0.5f);
    EXPECT_EQ(animation->getProgress(), 0.5f);
}
```

## Next Steps

1. Add post-processing effects
2. Implement clipping and masking
3. Add performance optimizations
4. Implement animation curves

## Common Pitfalls

1. **Animation Timing**: Handle frame rate independence
2. **State Transitions**: Manage complex state changes
3. **Property Updates**: Handle property type changes
4. **Performance**: Optimize animation updates

## Best Practices

1. Use appropriate easing functions
2. Handle animation interruptions
3. Support animation chaining
4. Optimize animation updates
5. Implement proper cleanup

## Additional Resources

- [Component Rendering Tutorial](../core/component_rendering.md)
- [Post-Processing Effects Tutorial](./post_processing.md)
- [Theme System Tutorial](../themes/theme_system.md)
- [Performance Optimization Tutorial](../optimization/performance_optimization.md)
- [Input Handling Tutorial](../input/input_handling.md)
