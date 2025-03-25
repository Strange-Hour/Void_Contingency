# Implementing Clipping Debugging

## Navigation

- Previous: [Clipping Optimizations Tutorial](./clipping_optimizations.md)
- Next: [Clipping Serialization Tutorial](./clipping_serialization.md)

This tutorial will guide you through implementing debugging tools for the clipping system in "Void Contingency", focusing on visualization, diagnostics, and performance profiling.

## Overview

The debugging system will:

- Visualize clipping regions
- Display transform hierarchies
- Show intersection tests
- Provide performance metrics
- Enable interactive debugging

## Implementation Steps

### 1. Create Clipping Debug Visualizer

Create `include/ui/debug/ClipDebugVisualizer.hpp`:

```cpp
#pragma once

#include "ui/clip/ClipCache.hpp"
#include "ui/render/Renderer.hpp"
#include <vector>
#include <memory>
#include <unordered_map>
#include <glm/glm.hpp>

namespace void_contingency {
namespace ui {

struct DebugVisualization {
    glm::vec4 color{1.0f, 0.0f, 0.0f, 0.5f};
    bool showBounds{true};
    bool showTransforms{true};
    bool showIntersections{true};
    bool showHierarchy{true};
    float lineWidth{2.0f};
};

class ClipDebugVisualizer {
public:
    static ClipDebugVisualizer& getInstance();

    // Visualization control
    void setVisualization(const DebugVisualization& viz);
    void toggleVisualization(bool enabled);
    bool isVisualizationEnabled() const { return enabled_; }

    // Region visualization
    void visualizeClipRect(const NestedClipRect& rect);
    void visualizeTransform(const glm::mat4& transform);
    void visualizeIntersection(const NestedClipRect& a, const NestedClipRect& b);
    void visualizeHierarchy(const NestedClipRect& root);

    // Debug rendering
    void renderDebugOverlay();
    void renderDebugText();

    // Interactive debugging
    void handleDebugInput(const InputEvent& event);
    void setSelectedComponent(ComponentPtr component);
    ComponentPtr getSelectedComponent() const { return selectedComponent_; }

private:
    ClipDebugVisualizer() = default;
    ~ClipDebugVisualizer() = default;
    ClipDebugVisualizer(const ClipDebugVisualizer&) = delete;
    ClipDebugVisualizer& operator=(const ClipDebugVisualizer&) = delete;

    // Debug state
    bool enabled_{false};
    DebugVisualization visualization_;
    ComponentPtr selectedComponent_;

    // Debug primitives
    struct DebugPrimitive {
        enum class Type {
            Line,
            Rectangle,
            Text
        } type;
        std::vector<glm::vec2> vertices;
        glm::vec4 color;
        std::string text;
    };
    std::vector<DebugPrimitive> primitives_;

    // Helper methods
    void renderPrimitive(const DebugPrimitive& primitive);
    void addDebugText(const std::string& text, const glm::vec2& position);
    void drawBounds(const NestedClipRect& rect);
    void drawTransform(const glm::mat4& transform);
    void drawIntersection(const NestedClipRect& a, const NestedClipRect& b);
    void drawHierarchy(const NestedClipRect& root, int depth = 0);
};

} // namespace ui
} // namespace void_contingency
```

### 2. Create Clipping Debug Profiler

Create `include/ui/debug/ClipDebugProfiler.hpp`:

```cpp
#pragma once

#include "ui/clip/ClipCache.hpp"
#include <vector>
#include <memory>
#include <chrono>
#include <string>

namespace void_contingency {
namespace ui {

struct ProfileSample {
    std::string name;
    float duration{0.0f};
    uint32_t count{0};
    std::vector<ProfileSample> children;
};

class ClipDebugProfiler {
public:
    static ClipDebugProfiler& getInstance();

    // Profiling control
    void beginFrame();
    void endFrame();
    void beginSample(const std::string& name);
    void endSample();

    // Performance metrics
    const ProfileSample& getFrameStats() const { return frameStats_; }
    float getAverageFrameTime() const { return averageFrameTime_; }
    uint32_t getFrameCount() const { return frameCount_; }

    // Debug output
    void printProfileStats();
    void saveProfileData(const std::string& filename);
    void loadProfileData(const std::string& filename);

private:
    ClipDebugProfiler() = default;
    ~ClipDebugProfiler() = default;
    ClipDebugProfiler(const ClipDebugProfiler&) = delete;
    ClipDebugProfiler& operator=(const ClipDebugProfiler&) = delete;

    // Profiling state
    ProfileSample frameStats_;
    std::vector<ProfileSample*> sampleStack_;
    std::chrono::high_resolution_clock::time_point frameStart_;
    float averageFrameTime_{0.0f};
    uint32_t frameCount_{0};

    // Helper methods
    void updateFrameStats();
    void calculateAverages(ProfileSample& sample);
    void printSample(const ProfileSample& sample, int depth = 0);
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create Clipping Debug Tests

Create `tests/unit/ui/debug/DebugTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/debug/ClipDebugVisualizer.hpp"
#include "ui/debug/ClipDebugProfiler.hpp"
#include "ui/components/ClippingTransform.hpp"

using namespace void_contingency::ui;

TEST(ClipDebugVisualizerTest, VisualizationControl) {
    auto& visualizer = ClipDebugVisualizer::getInstance();

    // Setup visualization
    DebugVisualization viz;
    viz.color = glm::vec4(1.0f, 0.0f, 0.0f, 0.5f);
    viz.showBounds = true;
    viz.showTransforms = true;
    visualizer.setVisualization(viz);

    // Toggle visualization
    visualizer.toggleVisualization(true);
    EXPECT_TRUE(visualizer.isVisualizationEnabled());

    // Create test component
    auto component = std::make_shared<ClippingTransform>();
    visualizer.setSelectedComponent(component);
    EXPECT_EQ(visualizer.getSelectedComponent(), component);
}

TEST(ClipDebugProfilerTest, ProfilingControl) {
    auto& profiler = ClipDebugProfiler::getInstance();

    // Begin frame
    profiler.beginFrame();

    // Profile samples
    profiler.beginSample("TestOperation");
    // Simulate work
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    profiler.endSample();

    // End frame
    profiler.endFrame();

    // Verify stats
    const auto& stats = profiler.getFrameStats();
    EXPECT_GT(stats.duration, 0.0f);
    EXPECT_EQ(stats.count, 1);
}

TEST(ClipDebugProfilerTest, PerformanceMetrics) {
    auto& profiler = ClipDebugProfiler::getInstance();

    // Profile multiple frames
    for (int i = 0; i < 3; ++i) {
        profiler.beginFrame();
        profiler.beginSample("FrameOperation");
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        profiler.endSample();
        profiler.endFrame();
    }

    // Verify metrics
    EXPECT_GT(profiler.getAverageFrameTime(), 0.0f);
    EXPECT_EQ(profiler.getFrameCount(), 3);
}
```

## Next Steps

1. Add clipping region serialization
2. Implement clipping region compression
3. Add clipping region profiling
4. Implement clipping region validation

## Common Pitfalls

1. **Visualization Performance**: Handle debug rendering overhead
2. **Profile Accuracy**: Ensure accurate timing measurements
3. **Memory Usage**: Manage debug data storage
4. **User Interface**: Design intuitive debug controls

## Best Practices

1. Use efficient debug rendering
2. Implement accurate profiling
3. Provide clear visualization
4. Support interactive debugging
5. Enable data export/import

## Additional Resources

- [UI Component System Tutorial](../core/ui_component_system.md)
- [Layout Management Tutorial](../core/layout_management.md)
- [Styling System Tutorial](../core/styling_system.md)
- [Component Rendering Tutorial](../core/component_rendering.md)
- [Component Animations Tutorial](../components/component_animations.md)
- [Post-Processing Effects Tutorial](../components/post_processing.md)
- [Clipping and Masking Tutorial](../clipping/clipping_masking.md)
- [Nested Clipping Tutorial](../clipping/nested_clipping.md)
- [Clipping Optimizations Tutorial](./clipping_optimizations.md)
- [Clipping Serialization Tutorial](./clipping_serialization.md)
- [Clipping Compression Tutorial](./clipping_compression.md)
