# Implementing Gesture Visualization Tools

## Navigation

- Previous: [Gesture Analysis Tutorial](./gesture_analysis.md)
- Next: [Gesture Templates Tutorial](./gesture_templates.md)

This tutorial will guide you through implementing gesture visualization tools for the UI framework in "Void Contingency", focusing on displaying gesture paths, metrics, and analysis data.

## Overview

The gesture visualization system will:

- Display gesture paths and trajectories
- Show performance metrics
- Visualize quality indicators
- Support pattern matching visualization
- Enable real-time visualization
- Provide interactive analysis views

## Implementation Steps

### 1. Create Gesture Visualization Types

Create `include/ui/input/GestureVisualization.hpp`:

```cpp
#pragma once

#include "ui/input/GestureAnalysis.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace void_contingency {
namespace ui {

struct VisualizationOptions {
    bool showPath{true};
    bool showPoints{true};
    bool showMetrics{true};
    bool showQuality{true};
    bool showPatterns{true};
    bool showPressure{true};
    bool showSpeed{true};
    glm::vec4 pathColor{1.0f, 1.0f, 1.0f, 1.0f};
    glm::vec4 pointColor{1.0f, 0.0f, 0.0f, 1.0f};
    glm::vec4 metricColor{0.0f, 1.0f, 0.0f, 1.0f};
    float pointSize{5.0f};
    float lineWidth{2.0f};
    float metricScale{1.0f};
};

struct VisualizationData {
    std::vector<glm::vec2> pathPoints;
    std::vector<float> pressureValues;
    std::vector<float> speedValues;
    std::vector<glm::vec2> metricPoints;
    std::vector<std::string> metricLabels;
    std::vector<glm::vec2> patternPoints;
    std::vector<std::string> patternLabels;
};

} // namespace ui
} // namespace void_contingency
```

### 2. Create Gesture Visualizer

Create `include/ui/input/GestureVisualizer.hpp`:

```cpp
#pragma once

#include "ui/input/GestureVisualization.hpp"
#include "ui/input/GestureAnalysis.hpp"
#include "ui/render/Renderer.hpp"
#include <memory>
#include <functional>

namespace void_contingency {
namespace ui {

using VisualizationCallback = std::function<void(const VisualizationData&)>;

class GestureVisualizer {
public:
    static GestureVisualizer& getInstance();

    // Visualization control
    void startVisualization(const VisualizationOptions& options = VisualizationOptions{});
    void stopVisualization();
    void updateVisualization(const GestureData& gesture);
    void updateAnalysis(const AnalysisReport& report);

    // Rendering
    void render(Renderer& renderer);
    void renderPath(Renderer& renderer);
    void renderPoints(Renderer& renderer);
    void renderMetrics(Renderer& renderer);
    void renderPatterns(Renderer& renderer);

    // Interactive features
    void setHighlightedPoint(size_t index);
    void setHighlightedPattern(const std::string& name);
    void setMetricScale(float scale);

    // Visualization subscription
    void subscribe(VisualizationCallback callback);
    void unsubscribe(VisualizationCallback callback);

private:
    GestureVisualizer() = default;
    ~GestureVisualizer() = default;
    GestureVisualizer(const GestureVisualizer&) = delete;
    GestureVisualizer& operator=(const GestureVisualizer&) = delete;

    // Visualization state
    VisualizationOptions options_;
    VisualizationData data_;
    size_t highlightedPoint_{0};
    std::string highlightedPattern_;
    std::vector<VisualizationCallback> callbacks_;

    // Helper methods
    void updatePathData(const GestureData& gesture);
    void updateMetricData(const AnalysisReport& report);
    void updatePatternData(const AnalysisReport& report);
    void notifyCallbacks(const VisualizationData& data);
    void drawLine(Renderer& renderer, const glm::vec2& start, const glm::vec2& end, const glm::vec4& color);
    void drawPoint(Renderer& renderer, const glm::vec2& point, const glm::vec4& color);
    void drawText(Renderer& renderer, const std::string& text, const glm::vec2& position, const glm::vec4& color);
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create Gesture View

Create `include/ui/input/GestureView.hpp`:

```cpp
#pragma once

#include "ui/input/GestureVisualization.hpp"
#include "ui/components/Component.hpp"
#include <memory>

namespace void_contingency {
namespace ui {

class GestureView : public Component {
public:
    GestureView();
    ~GestureView() override;

    // View control
    void setGesture(const GestureData& gesture);
    void setAnalysis(const AnalysisReport& report);
    void setOptions(const VisualizationOptions& options);

    // Interactive features
    void setInteractive(bool interactive) { interactive_ = interactive; }
    bool isInteractive() const { return interactive_; }

    // View customization
    void setBackgroundColor(const glm::vec4& color) { backgroundColor_ = color; }
    void setGridEnabled(bool enabled) { showGrid_ = enabled; }
    void setScale(float scale) { scale_ = scale; }

protected:
    void render(Renderer& renderer) override;
    void handleInput(const InputEvent& event) override;
    void update() override;

private:
    // View state
    GestureData currentGesture_;
    AnalysisReport currentAnalysis_;
    VisualizationOptions options_;
    glm::vec4 backgroundColor_{0.1f, 0.1f, 0.1f, 1.0f};
    bool showGrid_{true};
    float scale_{1.0f};
    bool interactive_{true};

    // Helper methods
    void renderBackground(Renderer& renderer);
    void renderGrid(Renderer& renderer);
    void handleMouseInput(const MouseEvent& event);
    void handleTouchInput(const TouchEvent& event);
    glm::vec2 transformPoint(const glm::vec2& point) const;
    glm::vec2 inverseTransformPoint(const glm::vec2& point) const;
};

} // namespace ui
} // namespace void_contingency
```

### 4. Create Visualization Tests

Create `tests/unit/ui/input/VisualizationTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/input/GestureVisualization.hpp"
#include "ui/input/GestureVisualizer.hpp"
#include "ui/input/GestureView.hpp"
#include "ui/render/Renderer.hpp"

using namespace void_contingency::ui;

TEST(GestureVisualizerTest, VisualizationControl) {
    auto& visualizer = GestureVisualizer::getInstance();

    // Start visualization
    VisualizationOptions options;
    options.showPath = true;
    options.showPoints = true;
    options.showMetrics = true;
    visualizer.startVisualization(options);

    // Create test gesture
    GestureData gesture;
    gesture.type = GestureType::Swipe;
    gesture.state = GestureState::Started;
    gesture.points = {
        {glm::vec2(0.0f), 1.0f, std::chrono::system_clock::now()},
        {glm::vec2(100.0f, 0.0f), 1.0f, std::chrono::system_clock::now() + std::chrono::milliseconds(100)}
    };

    // Update visualization
    visualizer.updateVisualization(gesture);

    // Create test analysis
    AnalysisReport report;
    report.metrics.averageSpeed = 1.0f;
    report.metrics.maxSpeed = 2.0f;
    report.quality.smoothness = 0.8f;

    // Update analysis
    visualizer.updateAnalysis(report);

    // Stop visualization
    visualizer.stopVisualization();
}

TEST(GestureViewTest, ViewRendering) {
    GestureView view;

    // Set gesture
    GestureData gesture;
    gesture.type = GestureType::Swipe;
    gesture.state = GestureState::Started;
    gesture.points = {
        {glm::vec2(0.0f), 1.0f, std::chrono::system_clock::now()},
        {glm::vec2(100.0f, 0.0f), 1.0f, std::chrono::system_clock::now() + std::chrono::milliseconds(100)}
    };
    view.setGesture(gesture);

    // Set analysis
    AnalysisReport report;
    report.metrics.averageSpeed = 1.0f;
    report.metrics.maxSpeed = 2.0f;
    report.quality.smoothness = 0.8f;
    view.setAnalysis(report);

    // Set options
    VisualizationOptions options;
    options.showPath = true;
    options.showPoints = true;
    options.showMetrics = true;
    view.setOptions(options);

    // Verify view state
    EXPECT_TRUE(view.isInteractive());
    view.setInteractive(false);
    EXPECT_FALSE(view.isInteractive());
}
```

## Next Steps

1. Add gesture sharing system
2. Implement gesture templates
3. Add gesture training system
4. Implement gesture export/import

## Common Pitfalls

1. **Rendering Performance**: Optimize drawing operations
2. **Interactive Features**: Handle input properly
3. **Visual Clarity**: Maintain clear visualization
4. **Memory Usage**: Manage visualization data

## Best Practices

1. Use efficient rendering
2. Implement proper scaling
3. Support customization
4. Enable interactivity
5. Provide clear feedback

## Additional Resources

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
- [Gesture Recording Tutorial](./gesture_recording.md)
- [Gesture Analysis Tutorial](./gesture_analysis.md)
