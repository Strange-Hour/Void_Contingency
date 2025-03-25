# Implementing HUD Benchmarking

## Navigation

- Previous: [HUD Profiling Tutorial](./hud_profiling.md)
- Next: [Input System](../input/input_handling.md)

This tutorial will guide you through implementing benchmarking tools for the HUD in "Void Contingency", enabling performance comparison and optimization analysis.

## Overview

The HUD benchmarking system will:

- Measure HUD performance
- Compare different configurations
- Analyze optimization impact
- Generate benchmark reports
- Support automated testing
- Enable performance tracking

## Implementation Steps

### 1. Create HUD Benchmarking Types

Create `include/ui/hud/HUDBenchmarking.hpp`:

```cpp
#pragma once

#include "ui/hud/HUDTypes.hpp"
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include <chrono>

namespace void_contingency {
namespace ui {

enum class BenchmarkType {
    Performance,
    Memory,
    Resource,
    Network,
    Graphics,
    Stress,
    Load
};

enum class BenchmarkStatus {
    NotStarted,
    Running,
    Paused,
    Completed,
    Failed
};

struct BenchmarkMetrics {
    float averageFrameTime{0.0f};
    float minFrameTime{0.0f};
    float maxFrameTime{0.0f};
    float frameTimeStdDev{0.0f};
    float averageRenderTime{0.0f};
    float averageUpdateTime{0.0f};
    float averageInputTime{0.0f};
    float averageLayoutTime{0.0f};
    float averageAnimationTime{0.0f};
    int totalDrawCalls{0};
    int averageDrawCalls{0};
    int totalVertices{0};
    int averageVertices{0};
    size_t totalMemoryUsage{0};
    size_t averageMemoryUsage{0};
    size_t peakMemoryUsage{0};
    float cpuUsage{0.0f};
    float gpuUsage{0.0f};
    float ramUsage{0.0f};
    float vramUsage{0.0f};
    std::vector<float> frameTimes;
    std::vector<float> renderTimes;
    std::vector<float> updateTimes;
    std::vector<size_t> memoryUsageHistory;
    std::vector<int> drawCallHistory;
    std::vector<int> vertexHistory;
};

struct BenchmarkConfig {
    std::string id;
    std::string name;
    std::string description;
    BenchmarkType type{BenchmarkType::Performance};
    std::chrono::milliseconds duration{60000};  // 1 minute default
    std::chrono::milliseconds warmupTime{5000}; // 5 seconds default
    std::chrono::milliseconds cooldownTime{5000}; // 5 seconds default
    int iterations{1};
    bool enableWarmup{true};
    bool enableCooldown{true};
    bool enableMetrics{true};
    bool enableHistory{true};
    bool enableAnalysis{true};
    bool enableReporting{true};
    std::vector<std::string> targets;
    std::unordered_map<std::string, std::string> parameters;
};

struct BenchmarkResult {
    std::string id;
    std::string configId;
    std::string name;
    std::chrono::system_clock::time_point timestamp;
    BenchmarkStatus status{BenchmarkStatus::NotStarted};
    std::chrono::milliseconds duration;
    std::vector<BenchmarkMetrics> metrics;
    std::vector<std::string> events;
    std::vector<std::string> warnings;
    std::vector<std::string> errors;
    float score{0.0f};
    std::string summary;
    std::string details;
    std::string recommendations;
    std::vector<std::string> bottlenecks;
    std::vector<std::string> optimizations;
    std::unordered_map<std::string, float> scores;
};

struct BenchmarkComparison {
    std::string id;
    std::string name;
    std::vector<std::string> resultIds;
    std::vector<BenchmarkResult> results;
    std::chrono::system_clock::time_point timestamp;
    std::string summary;
    std::string details;
    std::string recommendations;
    std::vector<std::string> differences;
    std::vector<std::string> improvements;
    std::vector<std::string> regressions;
    std::unordered_map<std::string, float> scoreDifferences;
};

struct BenchmarkOptions {
    bool enablePerformance{true};
    bool enableMemory{true};
    bool enableResources{true};
    bool enableNetwork{true};
    bool enableGraphics{true};
    bool enableEvents{true};
    bool enableWarnings{true};
    bool enableErrors{true};
    bool enableHistory{true};
    bool enableAnalysis{true};
    bool enableReporting{true};
    bool enableComparison{true};
    std::chrono::milliseconds defaultDuration{60000};
    std::chrono::milliseconds defaultWarmupTime{5000};
    std::chrono::milliseconds defaultCooldownTime{5000};
    int defaultIterations{1};
    std::string outputDir{"benchmark_output"};
    std::vector<BenchmarkType> types;
    std::unordered_map<std::string, std::string> parameters;
};

using BenchmarkCallback = std::function<void(const BenchmarkResult&)>;

} // namespace ui
} // namespace void_contingency
```

### 2. Create HUD Benchmarking Manager

Create `include/ui/hud/HUDBenchmarkingManager.hpp`:

```cpp
#pragma once

#include "ui/hud/HUDBenchmarking.hpp"
#include <memory>
#include <vector>
#include <unordered_map>

namespace void_contingency {
namespace ui {

class HUDBenchmarkingManager {
public:
    static HUDBenchmarkingManager& getInstance();

    // Configuration management
    void addConfig(const BenchmarkConfig& config);
    void removeConfig(const std::string& id);
    void updateConfig(const std::string& id, const BenchmarkConfig& config);
    BenchmarkConfig getConfig(const std::string& id) const;

    // Benchmark execution
    void startBenchmark(const std::string& configId);
    void stopBenchmark(const std::string& id);
    void pauseBenchmark(const std::string& id);
    void resumeBenchmark(const std::string& id);
    void resetBenchmark(const std::string& id);

    // Metrics collection
    void collectMetrics();
    void trackFrameTime(float time);
    void trackRenderTime(float time);
    void trackUpdateTime(float time);
    void trackMemoryUsage(size_t usage);
    void trackDrawCalls(int count);
    void trackVertices(int count);
    void trackEvent(const std::string& event);
    void trackWarning(const std::string& warning);
    void trackError(const std::string& error);

    // Result management
    BenchmarkResult getResult(const std::string& id) const;
    void saveResult(const std::string& path);
    void loadResult(const std::string& path);
    void exportResult(const std::string& format, const std::string& path);

    // Comparison
    BenchmarkComparison compareResults(const std::vector<std::string>& resultIds);
    void saveComparison(const std::string& path);
    void loadComparison(const std::string& path);
    void exportComparison(const std::string& format, const std::string& path);

    // Benchmark subscription
    void subscribeToBenchmark(BenchmarkCallback callback);
    void unsubscribeFromBenchmark(BenchmarkCallback callback);

private:
    HUDBenchmarkingManager() = default;
    ~HUDBenchmarkingManager() = default;
    HUDBenchmarkingManager(const HUDBenchmarkingManager&) = delete;
    HUDBenchmarkingManager& operator=(const HUDBenchmarkingManager&) = delete;

    // Benchmarking state
    std::unordered_map<std::string, BenchmarkConfig> configs_;
    std::unordered_map<std::string, BenchmarkResult> results_;
    std::vector<BenchmarkCallback> callbacks_;
    BenchmarkOptions options_;
    bool isRunning_{false};
    bool isPaused_{false};

    // Helper methods
    void updateMetrics();
    void analyzeMetrics();
    void detectBottlenecks();
    void generateOptimizations();
    void calculateScores();
    void notifyCallbacks(const BenchmarkResult& result);
    void cleanupResources();
    void manageHistory();
    void validateData();
    void backupData();
    void restoreData();
    void warmup();
    void cooldown();
    void runIteration();
    void processResults();
    void generateReport();
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create HUD Benchmarking View

Create `include/ui/hud/HUDBenchmarkingView.hpp`:

```cpp
#pragma once

#include "ui/hud/HUDBenchmarking.hpp"
#include "ui/components/Component.hpp"
#include <memory>

namespace void_contingency {
namespace ui {

class HUDBenchmarkingView : public Component {
public:
    HUDBenchmarkingView();
    ~HUDBenchmarkingView() override;

    // View control
    void setConfig(const BenchmarkConfig& config);
    void setResult(const BenchmarkResult& result);
    void setComparison(const BenchmarkComparison& comparison);

    // Interactive features
    void setInteractive(bool interactive) { interactive_ = interactive; }
    bool isInteractive() const { return interactive_; }

    // View customization
    void setShowConfig(bool show) { showConfig_ = show; }
    void setShowMetrics(bool show) { showMetrics_ = show; }
    void setShowResults(bool show) { showResults_ = show; }
    void setShowComparison(bool show) { showComparison_ = show; }
    void setShowControls(bool show) { showControls_ = show; }

protected:
    void render(Renderer& renderer) override;
    void handleInput(const InputEvent& event) override;
    void update() override;

private:
    // View state
    BenchmarkConfig config_;
    BenchmarkResult result_;
    BenchmarkComparison comparison_;
    bool interactive_{false};
    bool showConfig_{true};
    bool showMetrics_{true};
    bool showResults_{true};
    bool showComparison_{true};
    bool showControls_{true};

    // Helper methods
    void renderConfig(Renderer& renderer);
    void renderMetrics(Renderer& renderer);
    void renderResults(Renderer& renderer);
    void renderComparison(Renderer& renderer);
    void renderControls(Renderer& renderer);
    void handleBenchmarkingInput(const InputEvent& event);
    void updateVisualization();
};

} // namespace ui
} // namespace void_contingency
```

### 4. Create Benchmarking Tests

Create `tests/unit/ui/hud/BenchmarkingTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/hud/HUDBenchmarking.hpp"
#include "ui/hud/HUDBenchmarkingManager.hpp"
#include "ui/hud/HUDBenchmarkingView.hpp"

using namespace void_contingency::ui;

TEST(HUDBenchmarkingManagerTest, ConfigurationManagement) {
    auto& manager = HUDBenchmarkingManager::getInstance();

    // Create test config
    BenchmarkConfig config;
    config.id = "test_config";
    config.name = "Test Config";
    config.description = "Test configuration for HUD benchmarking";
    config.type = BenchmarkType::Performance;
    config.duration = std::chrono::milliseconds(10000);  // 10 seconds
    config.warmupTime = std::chrono::milliseconds(1000); // 1 second
    config.cooldownTime = std::chrono::milliseconds(1000); // 1 second
    config.iterations = 1;

    // Add config
    manager.addConfig(config);

    // Verify config
    auto retrievedConfig = manager.getConfig("test_config");
    EXPECT_EQ(retrievedConfig.id, "test_config");
    EXPECT_EQ(retrievedConfig.name, "Test Config");
    EXPECT_EQ(retrievedConfig.type, BenchmarkType::Performance);
}

TEST(HUDBenchmarkingManagerTest, BenchmarkExecution) {
    auto& manager = HUDBenchmarkingManager::getInstance();

    // Start benchmark
    manager.startBenchmark("test_config");

    // Track metrics
    manager.trackFrameTime(16.67f);
    manager.trackRenderTime(8.33f);
    manager.trackMemoryUsage(100 * 1024 * 1024);  // 100MB
    manager.trackDrawCalls(100);
    manager.trackVertices(10000);

    // Stop benchmark
    manager.stopBenchmark("test_benchmark");

    // Verify result
    auto result = manager.getResult("test_benchmark");
    EXPECT_EQ(result.status, BenchmarkStatus::Completed);
    EXPECT_GT(result.score, 0.0f);
}

TEST(HUDBenchmarkingManagerTest, ResultComparison) {
    auto& manager = HUDBenchmarkingManager::getInstance();

    // Create test results
    std::vector<std::string> resultIds = {"result1", "result2"};

    // Compare results
    auto comparison = manager.compareResults(resultIds);

    // Verify comparison
    EXPECT_FALSE(comparison.differences.empty());
    EXPECT_FALSE(comparison.improvements.empty());
    EXPECT_FALSE(comparison.regressions.empty());
}

TEST(HUDBenchmarkingViewTest, ViewRendering) {
    HUDBenchmarkingView view;

    // Set test config
    BenchmarkConfig config;
    config.id = "test_config";
    config.name = "Test Config";
    view.setConfig(config);

    // Set test result
    BenchmarkResult result;
    result.id = "test_result";
    result.configId = "test_config";
    view.setResult(result);

    // Verify view state
    EXPECT_FALSE(view.isInteractive());
    view.setInteractive(true);
    EXPECT_TRUE(view.isInteractive());
}
```

## Next Steps

1. Add HUD analytics
2. Implement HUD optimization
3. Add HUD sharing
4. Implement HUD testing

## Common Pitfalls

1. **Benchmark Accuracy**: Ensure consistent test conditions
2. **Resource Usage**: Control benchmark overhead
3. **Data Management**: Handle large datasets
4. **Result Analysis**: Provide meaningful insights

## Best Practices

1. Use proper warmup/cooldown
2. Implement multiple iterations
3. Support various metrics
4. Enable result comparison
5. Provide clear reports

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
- [Menu System Tutorial](../menu/menu_system.md)
