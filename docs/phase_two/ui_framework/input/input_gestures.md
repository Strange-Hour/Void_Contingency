# Implementing Input Gestures

## Navigation

- Previous: [Input Handling Tutorial](./input_handling.md)
- Next: [Gesture Recording Tutorial](../gestures/gesture_recording.md)

This tutorial will guide you through implementing input gestures for "Void Contingency", providing gesture recognition and processing capabilities.

## Overview

The gesture recognition system will:

- Detect common gestures (tap, swipe, pinch, rotate)
- Track gesture state and progress
- Support gesture callbacks
- Enable gesture validation
- Handle gesture cancellation
- Support gesture thresholds

## Implementation Steps

### 1. Create Gesture Types

Create `include/ui/input/GestureTypes.hpp`:

```cpp
#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace void_contingency {
namespace ui {

enum class GestureType {
    Tap,
    Swipe,
    Pinch,
    Rotate,
    Pan,
    LongPress
};

enum class GestureState {
    None,
    Started,
    Updated,
    Completed,
    Cancelled
};

struct GesturePoint {
    glm::vec2 position;
    float pressure;
    std::chrono::system_clock::time_point timestamp;
};

struct GestureData {
    GestureType type;
    GestureState state;
    std::vector<GesturePoint> points;
    glm::vec2 center;
    float scale;
    float rotation;
    glm::vec2 translation;
    float duration;
};

} // namespace ui
} // namespace void_contingency
```

### 2. Create Gesture Detector

Create `include/ui/input/GestureDetector.hpp`:

```cpp
#pragma once

#include "ui/input/GestureTypes.hpp"
#include "ui/input/InputEvent.hpp"
#include <unordered_map>
#include <functional>

namespace void_contingency {
namespace ui {

using GestureCallback = std::function<void(const GestureData&)>;

class GestureDetector {
public:
    static GestureDetector& getInstance();

    // Gesture detection
    void processEvent(const InputEvent& event);
    void update();

    // Gesture subscription
    void subscribe(GestureType type, GestureCallback callback);
    void unsubscribe(GestureType type, GestureCallback callback);

    // Gesture configuration
    void setTapThreshold(float threshold) { tapThreshold_ = threshold; }
    void setSwipeThreshold(float threshold) { swipeThreshold_ = threshold; }
    void setPinchThreshold(float threshold) { pinchThreshold_ = threshold; }
    void setRotationThreshold(float threshold) { rotationThreshold_ = threshold; }
    void setLongPressDuration(float duration) { longPressDuration_ = duration; }

    // Gesture state
    bool isGestureActive(GestureType type) const;
    const GestureData& getGestureData(GestureType type) const;
    void cancelGesture(GestureType type);

private:
    GestureDetector() = default;
    ~GestureDetector() = default;
    GestureDetector(const GestureDetector&) = delete;
    GestureDetector& operator=(const GestureDetector&) = delete;

    // Gesture state
    std::unordered_map<GestureType, GestureData> gestures_;
    std::unordered_map<GestureType, std::vector<GestureCallback>> callbacks_;

    // Configuration
    float tapThreshold_{5.0f};
    float swipeThreshold_{50.0f};
    float pinchThreshold_{0.1f};
    float rotationThreshold_{0.1f};
    float longPressDuration_{0.5f};

    // Helper methods
    void detectTap(const std::vector<GesturePoint>& points);
    void detectSwipe(const std::vector<GesturePoint>& points);
    void detectPinch(const std::vector<GesturePoint>& points);
    void detectRotate(const std::vector<GesturePoint>& points);
    void detectPan(const std::vector<GesturePoint>& points);
    void detectLongPress(const std::vector<GesturePoint>& points);

    void updateGestureState(GestureType type, GestureState state);
    void notifyGestureCallbacks(GestureType type, const GestureData& data);
    bool validateGesture(const GestureData& data) const;
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create Gesture Manager

Create `include/ui/input/GestureManager.hpp`:

```cpp
#pragma once

#include "ui/input/GestureDetector.hpp"
#include "ui/components/Component.hpp"
#include <unordered_map>

namespace void_contingency {
namespace ui {

class GestureManager {
public:
    static GestureManager& getInstance();

    // Gesture management
    void processEvent(const InputEvent& event);
    void update();

    // Component gesture handling
    void enableGestures(Component* component, const std::vector<GestureType>& types);
    void disableGestures(Component* component);
    bool hasGestures(Component* component) const;

    // Gesture priority
    void setGesturePriority(Component* component, int priority);
    int getGesturePriority(Component* component) const;

    // Gesture propagation
    void setPropagationEnabled(bool enabled) { propagationEnabled_ = enabled; }
    bool isPropagationEnabled() const { return propagationEnabled_; }

private:
    GestureManager() = default;
    ~GestureManager() = default;
    GestureManager(const GestureManager&) = delete;
    GestureManager& operator=(const GestureManager&) = delete;

    // Component gesture state
    struct ComponentGestures {
        std::vector<GestureType> enabledTypes;
        int priority{0};
        bool active{false};
    };
    std::unordered_map<Component*, ComponentGestures> componentGestures_;

    // Gesture state
    bool propagationEnabled_{true};
    Component* activeComponent_{nullptr};

    // Helper methods
    void updateComponentGestures();
    bool handleComponentGesture(Component* component, const GestureData& data);
    Component* findGestureTarget(const glm::vec2& position) const;
    void propagateGesture(const GestureData& data);
};

} // namespace ui
} // namespace void_contingency
```

### 4. Create Gesture Tests

Create `tests/unit/ui/input/GestureTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/input/GestureTypes.hpp"
#include "ui/input/GestureDetector.hpp"
#include "ui/input/GestureManager.hpp"
#include "ui/components/Component.hpp"

using namespace void_contingency::ui;

TEST(GestureDetectorTest, TapDetection) {
    auto& detector = GestureDetector::getInstance();

    // Create tap gesture points
    std::vector<GesturePoint> points;
    points.push_back({glm::vec2(0.0f), 1.0f, std::chrono::system_clock::now()});
    points.push_back({glm::vec2(0.0f), 0.0f, std::chrono::system_clock::now() + std::chrono::milliseconds(100)});

    // Detect tap
    detector.detectTap(points);

    // Verify gesture
    EXPECT_TRUE(detector.isGestureActive(GestureType::Tap));
    const auto& data = detector.getGestureData(GestureType::Tap);
    EXPECT_EQ(data.state, GestureState::Completed);
}

TEST(GestureDetectorTest, SwipeDetection) {
    auto& detector = GestureDetector::getInstance();

    // Create swipe gesture points
    std::vector<GesturePoint> points;
    points.push_back({glm::vec2(0.0f), 1.0f, std::chrono::system_clock::now()});
    points.push_back({glm::vec2(100.0f, 0.0f), 1.0f, std::chrono::system_clock::now() + std::chrono::milliseconds(100)});

    // Detect swipe
    detector.detectSwipe(points);

    // Verify gesture
    EXPECT_TRUE(detector.isGestureActive(GestureType::Swipe));
    const auto& data = detector.getGestureData(GestureType::Swipe);
    EXPECT_EQ(data.state, GestureState::Completed);
}

TEST(GestureManagerTest, ComponentGestures) {
    auto& manager = GestureManager::getInstance();

    // Create component
    Component component;

    // Enable gestures
    std::vector<GestureType> types{GestureType::Tap, GestureType::Swipe};
    manager.enableGestures(&component, types);

    // Verify gestures
    EXPECT_TRUE(manager.hasGestures(&component));

    // Set priority
    manager.setGesturePriority(&component, 1);
    EXPECT_EQ(manager.getGesturePriority(&component), 1);

    // Disable gestures
    manager.disableGestures(&component);
    EXPECT_FALSE(manager.hasGestures(&component));
}
```

## Next Steps

1. Add gesture recording and playback
2. Implement gesture configuration system
3. Add gesture debugging tools
4. Implement gesture validation rules

## Common Pitfalls

1. **Gesture Detection**: Handle edge cases
2. **State Management**: Maintain consistent state
3. **Gesture Priority**: Handle overlapping gestures
4. **Performance**: Optimize gesture detection

## Best Practices

1. Use gesture thresholds
2. Implement proper state management
3. Support gesture cancellation
4. Handle gesture priority
5. Validate gesture data

## Additional Resources

- [Input Handling Tutorial](./input_handling.md)
- [Gesture Recording Tutorial](./gesture_recording.md)
- [Gesture Analysis Tutorial](./gesture_analysis.md)
- [Gesture Visualization Tutorial](./gesture_visualization.md)
- [Gesture Templates Tutorial](./gesture_templates.md)
- [Gesture Training Tutorial](./gesture_training.md)
- [Gesture Export/Import Tutorial](./gesture_export_import.md)
- [Gesture Collaboration Tutorial](./gesture_collaboration.md)
- [Gesture Recognition Tutorial](./gesture_recognition.md)
- [Gesture Customization Tutorial](./gesture_customization.md)
- [Gesture Validation Tutorial](./gesture_validation.md)
- [Gesture Analytics Tutorial](./gesture_analytics.md)
- [Gesture Optimization Tutorial](./gesture_optimization.md)
- [Gesture Sharing Tutorial](./gesture_sharing.md)
