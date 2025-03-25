# Implementing Gesture Training System

## Navigation

- Previous: [Gesture Templates Tutorial](./gesture_templates.md)
- Next: [Gesture Export/Import Tutorial](./gesture_export_import.md)

This tutorial will guide you through implementing a gesture training system for the UI framework in "Void Contingency", focusing on helping players learn and master gesture inputs.

## Overview

The gesture training system will:

- Manage training sessions and exercises
- Track player progress and performance
- Provide real-time feedback
- Support progressive learning
- Enable custom training exercises
- Track mastery of gesture types

## Implementation Steps

### 1. Create Gesture Training Types

Create `include/ui/input/GestureTraining.hpp`:

```cpp
#pragma once

#include "ui/input/GestureTemplate.hpp"
#include <string>
#include <vector>
#include <chrono>
#include <optional>

namespace void_contingency {
namespace ui {

enum class TrainingDifficulty {
    Beginner,
    Intermediate,
    Advanced,
    Expert
};

enum class TrainingProgress {
    NotStarted,
    InProgress,
    Completed,
    Mastered
};

struct TrainingExercise {
    std::string exerciseId;
    std::string name;
    std::string description;
    std::string templateId;
    TrainingDifficulty difficulty;
    std::vector<std::string> prerequisites;
    float requiredAccuracy{0.8f};
    float requiredSpeed{1.0f};
    int requiredAttempts{3};
    std::chrono::seconds timeLimit{30};
};

struct TrainingSession {
    std::string sessionId;
    std::string name;
    std::string description;
    std::vector<std::string> exerciseIds;
    TrainingDifficulty difficulty;
    bool isCustom{false};
    std::chrono::system_clock::time_point createdAt;
    std::chrono::system_clock::time_point lastPlayed;
};

struct TrainingProgress {
    std::string exerciseId;
    int attempts{0};
    int successfulAttempts{0};
    float bestAccuracy{0.0f};
    float bestSpeed{0.0f};
    std::chrono::system_clock::time_point lastAttempt;
    bool isCompleted{false};
    bool isMastered{false};
};

struct TrainingFeedback {
    float accuracy{0.0f};
    float speed{0.0f};
    float pressure{0.0f};
    std::vector<std::string> issues;
    std::vector<std::string> suggestions;
    bool isSuccessful{false};
};

} // namespace ui
} // namespace void_contingency
```

### 2. Create Gesture Training Manager

Create `include/ui/input/GestureTrainingManager.hpp`:

```cpp
#pragma once

#include "ui/input/GestureTraining.hpp"
#include "ui/input/GestureTemplate.hpp"
#include <memory>
#include <functional>
#include <unordered_map>

namespace void_contingency {
namespace ui {

using TrainingCallback = std::function<void(const TrainingProgress&)>;
using FeedbackCallback = std::function<void(const TrainingFeedback&)>;

class GestureTrainingManager {
public:
    static GestureTrainingManager& getInstance();

    // Training session management
    std::string createSession(const TrainingSession& session);
    void startSession(const std::string& sessionId);
    void pauseSession(const std::string& sessionId);
    void resumeSession(const std::string& sessionId);
    void endSession(const std::string& sessionId);

    // Exercise management
    std::string createExercise(const TrainingExercise& exercise);
    void updateExercise(const std::string& exerciseId, const TrainingExercise& exercise);
    void deleteExercise(const std::string& exerciseId);
    void skipExercise(const std::string& exerciseId);

    // Progress tracking
    TrainingProgress getProgress(const std::string& exerciseId) const;
    std::vector<std::string> getCompletedExercises() const;
    std::vector<std::string> getMasteredExercises() const;
    float getOverallProgress() const;

    // Feedback generation
    TrainingFeedback evaluateAttempt(const std::string& exerciseId, const RecordedGesture& gesture);
    void provideFeedback(const std::string& exerciseId, const TrainingFeedback& feedback);

    // Training subscription
    void subscribe(TrainingCallback callback);
    void unsubscribe(TrainingCallback callback);
    void subscribeToFeedback(FeedbackCallback callback);
    void unsubscribeFromFeedback(FeedbackCallback callback);

private:
    GestureTrainingManager() = default;
    ~GestureTrainingManager() = default;
    GestureTrainingManager(const GestureTrainingManager&) = delete;
    GestureTrainingManager& operator=(const GestureTrainingManager&) = delete;

    // Training state
    std::unordered_map<std::string, TrainingSession> sessions_;
    std::unordered_map<std::string, TrainingExercise> exercises_;
    std::unordered_map<std::string, TrainingProgress> progress_;
    std::string currentSessionId_;
    std::string currentExerciseId_;
    std::vector<TrainingCallback> trainingCallbacks_;
    std::vector<FeedbackCallback> feedbackCallbacks_;

    // Helper methods
    void notifyTrainingCallbacks(const TrainingProgress& progress);
    void notifyFeedbackCallbacks(const TrainingFeedback& feedback);
    void updateProgress(const std::string& exerciseId, const TrainingFeedback& feedback);
    bool checkPrerequisites(const std::string& exerciseId) const;
    void generateFeedback(const std::string& exerciseId, const RecordedGesture& gesture, TrainingFeedback& feedback);
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create Gesture Training View

Create `include/ui/input/GestureTrainingView.hpp`:

```cpp
#pragma once

#include "ui/input/GestureTraining.hpp"
#include "ui/components/Component.hpp"
#include <memory>

namespace void_contingency {
namespace ui {

class GestureTrainingView : public Component {
public:
    GestureTrainingView();
    ~GestureTrainingView() override;

    // View control
    void setSessionId(const std::string& sessionId);
    void setExerciseId(const std::string& exerciseId);
    void setProgress(const TrainingProgress& progress);
    void setFeedback(const TrainingFeedback& feedback);

    // Interactive features
    void setInteractive(bool interactive) { interactive_ = interactive; }
    bool isInteractive() const { return interactive_; }

    // View customization
    void setShowProgress(bool show) { showProgress_ = show; }
    void setShowFeedback(bool show) { showFeedback_ = show; }
    void setShowGuide(bool show) { showGuide_ = show; }

protected:
    void render(Renderer& renderer) override;
    void handleInput(const InputEvent& event) override;
    void update() override;

private:
    // View state
    std::string currentSessionId_;
    std::string currentExerciseId_;
    TrainingProgress currentProgress_;
    TrainingFeedback currentFeedback_;
    bool interactive_{false};
    bool showProgress_{true};
    bool showFeedback_{true};
    bool showGuide_{true};

    // Helper methods
    void renderProgress(Renderer& renderer);
    void renderFeedback(Renderer& renderer);
    void renderGuide(Renderer& renderer);
    void handleTrainingInput(const InputEvent& event);
    void handleFeedbackInput(const InputEvent& event);
    void handleGuideInput(const InputEvent& event);
    void updateGuide();
};

} // namespace ui
} // namespace void_contingency
```

### 4. Create Training Tests

Create `tests/unit/ui/input/TrainingTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/input/GestureTraining.hpp"
#include "ui/input/GestureTrainingManager.hpp"
#include "ui/input/GestureTrainingView.hpp"

using namespace void_contingency::ui;

TEST(GestureTrainingManagerTest, TrainingManagement) {
    auto& manager = GestureTrainingManager::getInstance();

    // Create test session
    TrainingSession session;
    session.name = "Basic Gestures";
    session.description = "Learn basic gesture controls";
    session.difficulty = TrainingDifficulty::Beginner;

    // Create session
    std::string sessionId = manager.createSession(session);

    // Create test exercise
    TrainingExercise exercise;
    exercise.name = "Basic Swipe";
    exercise.description = "Practice basic swipe gesture";
    exercise.templateId = "swipe_template";
    exercise.difficulty = TrainingDifficulty::Beginner;
    exercise.requiredAccuracy = 0.8f;
    exercise.requiredSpeed = 1.0f;

    // Create exercise
    std::string exerciseId = manager.createExercise(exercise);

    // Start session
    manager.startSession(sessionId);

    // Create test gesture
    RecordedGesture gesture;
    gesture.id = "test_gesture";
    gesture.type = GestureType::Swipe;
    gesture.points = {
        {glm::vec2(0.0f), 1.0f, std::chrono::system_clock::now()},
        {glm::vec2(100.0f, 0.0f), 1.0f, std::chrono::system_clock::now() + std::chrono::milliseconds(100)}
    };

    // Evaluate attempt
    auto feedback = manager.evaluateAttempt(exerciseId, gesture);
    EXPECT_GE(feedback.accuracy, 0.0f);

    // Update progress
    manager.provideFeedback(exerciseId, feedback);

    // Verify progress
    auto progress = manager.getProgress(exerciseId);
    EXPECT_EQ(progress.attempts, 1);

    // End session
    manager.endSession(sessionId);
}

TEST(GestureTrainingViewTest, ViewRendering) {
    GestureTrainingView view;

    // Set test data
    std::string sessionId = "test_session";
    view.setSessionId(sessionId);

    std::string exerciseId = "test_exercise";
    view.setExerciseId(exerciseId);

    TrainingProgress progress;
    progress.exerciseId = exerciseId;
    progress.attempts = 1;
    progress.successfulAttempts = 1;
    progress.bestAccuracy = 0.9f;
    view.setProgress(progress);

    TrainingFeedback feedback;
    feedback.accuracy = 0.9f;
    feedback.speed = 1.0f;
    feedback.isSuccessful = true;
    view.setFeedback(feedback);

    // Verify view state
    EXPECT_FALSE(view.isInteractive());
    view.setInteractive(true);
    EXPECT_TRUE(view.isInteractive());
}
```

## Next Steps

1. Implement gesture export/import
2. Add gesture collaboration features
3. Implement gesture recognition system
4. Add gesture customization options

## Common Pitfalls

1. **Progress Tracking**: Handle edge cases properly
2. **Feedback Generation**: Ensure accurate feedback
3. **Session Management**: Handle interruptions
4. **Performance**: Optimize training evaluation

## Best Practices

1. Use efficient evaluation
2. Implement proper validation
3. Support customization
4. Enable progress tracking
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
- [Gesture Visualization Tutorial](./gesture_visualization.md)
- [Gesture Sharing Tutorial](./gesture_sharing.md)
- [Gesture Templates Tutorial](./gesture_templates.md)
