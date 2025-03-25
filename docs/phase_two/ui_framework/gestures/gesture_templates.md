# Implementing Gesture Templates

## Navigation

- Previous: [Gesture Visualization Tutorial](./gesture_visualization.md)
- Next: [Gesture Training Tutorial](./gesture_training.md)

This tutorial will guide you through implementing a gesture template system for the UI framework in "Void Contingency", focusing on creating, managing, and reusing common gesture patterns.

## Overview

The gesture template system will:

- Create and manage gesture templates
- Support template categories and tags
- Enable template preview and editing
- Allow template sharing and collaboration
- Support template versioning
- Enable template validation

## Implementation Steps

### 1. Create Gesture Template Types

Create `include/ui/input/GestureTemplate.hpp`:

```cpp
#pragma once

#include "ui/input/GestureRecording.hpp"
#include <string>
#include <vector>
#include <chrono>
#include <optional>

namespace void_contingency {
namespace ui {

enum class TemplateCategory {
    Navigation,
    Combat,
    Menu,
    Custom
};

struct TemplateMetadata {
    std::string templateId;
    std::string name;
    std::string description;
    TemplateCategory category;
    std::vector<std::string> tags;
    std::string authorId;
    std::chrono::system_clock::time_point createdAt;
    std::chrono::system_clock::time_point modifiedAt;
    std::string version;
    bool isPublic{false};
    bool isVerified{false};
};

struct TemplateValidation {
    float minSpeed{0.0f};
    float maxSpeed{std::numeric_limits<float>::max()};
    float minPressure{0.0f};
    float maxPressure{1.0f};
    float minDuration{0.0f};
    float maxDuration{std::numeric_limits<float>::max()};
    std::vector<glm::vec2> requiredPoints;
    std::vector<glm::vec2> forbiddenPoints;
};

struct TemplatePreview {
    std::vector<glm::vec2> previewPoints;
    std::vector<float> previewPressure;
    std::vector<float> previewSpeed;
    float duration{0.0f};
    bool isValid{false};
};

} // namespace ui
} // namespace void_contingency
```

### 2. Create Gesture Template Manager

Create `include/ui/input/GestureTemplateManager.hpp`:

```cpp
#pragma once

#include "ui/input/GestureTemplate.hpp"
#include "ui/input/GestureRecording.hpp"
#include <memory>
#include <functional>
#include <unordered_map>

namespace void_contingency {
namespace ui {

using TemplateCallback = std::function<void(const TemplateMetadata&)>;
using PreviewCallback = std::function<void(const TemplatePreview&)>;

class GestureTemplateManager {
public:
    static GestureTemplateManager& getInstance();

    // Template management
    std::string createTemplate(const RecordedGesture& gesture, const TemplateMetadata& metadata);
    void updateTemplate(const std::string& templateId, const RecordedGesture& gesture);
    void deleteTemplate(const std::string& templateId);
    void duplicateTemplate(const std::string& templateId, const std::string& newName);

    // Template retrieval
    RecordedGesture getTemplate(const std::string& templateId) const;
    TemplateMetadata getMetadata(const std::string& templateId) const;
    std::vector<std::string> getTemplatesByCategory(TemplateCategory category) const;
    std::vector<std::string> getTemplatesByTag(const std::string& tag) const;

    // Template validation
    void setValidation(const std::string& templateId, const TemplateValidation& validation);
    bool validateGesture(const std::string& templateId, const RecordedGesture& gesture) const;
    TemplatePreview generatePreview(const std::string& templateId) const;

    // Template sharing
    void shareTemplate(const std::string& templateId);
    void unshareTemplate(const std::string& templateId);
    void verifyTemplate(const std::string& templateId);

    // Template subscription
    void subscribe(TemplateCallback callback);
    void unsubscribe(TemplateCallback callback);
    void subscribeToPreview(PreviewCallback callback);
    void unsubscribeFromPreview(PreviewCallback callback);

private:
    GestureTemplateManager() = default;
    ~GestureTemplateManager() = default;
    GestureTemplateManager(const GestureTemplateManager&) = delete;
    GestureTemplateManager& operator=(const GestureTemplateManager&) = delete;

    // Template state
    std::unordered_map<std::string, RecordedGesture> templates_;
    std::unordered_map<std::string, TemplateMetadata> metadata_;
    std::unordered_map<std::string, TemplateValidation> validation_;
    std::vector<TemplateCallback> templateCallbacks_;
    std::vector<PreviewCallback> previewCallbacks_;

    // Helper methods
    void notifyTemplateCallbacks(const TemplateMetadata& metadata);
    void notifyPreviewCallbacks(const TemplatePreview& preview);
    void validateTemplate(const std::string& templateId);
    void updateVersion(const std::string& templateId);
    void generatePreviewData(const std::string& templateId, TemplatePreview& preview);
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create Gesture Template View

Create `include/ui/input/GestureTemplateView.hpp`:

```cpp
#pragma once

#include "ui/input/GestureTemplate.hpp"
#include "ui/components/Component.hpp"
#include <memory>

namespace void_contingency {
namespace ui {

class GestureTemplateView : public Component {
public:
    GestureTemplateView();
    ~GestureTemplateView() override;

    // View control
    void setTemplateId(const std::string& templateId);
    void setMetadata(const TemplateMetadata& metadata);
    void setValidation(const TemplateValidation& validation);

    // Interactive features
    void setEditable(bool editable) { editable_ = editable; }
    bool isEditable() const { return editable_; }

    // View customization
    void setShowPreview(bool show) { showPreview_ = show; }
    void setShowValidation(bool show) { showValidation_ = show; }
    void setShowMetadata(bool show) { showMetadata_ = show; }

protected:
    void render(Renderer& renderer) override;
    void handleInput(const InputEvent& event) override;
    void update() override;

private:
    // View state
    std::string currentTemplateId_;
    TemplateMetadata currentMetadata_;
    TemplateValidation currentValidation_;
    TemplatePreview currentPreview_;
    bool editable_{false};
    bool showPreview_{true};
    bool showValidation_{true};
    bool showMetadata_{true};

    // Helper methods
    void renderPreview(Renderer& renderer);
    void renderValidation(Renderer& renderer);
    void renderMetadata(Renderer& renderer);
    void handleTemplateInput(const InputEvent& event);
    void handleValidationInput(const InputEvent& event);
    void handleMetadataInput(const InputEvent& event);
    void updatePreview();
};

} // namespace ui
} // namespace void_contingency
```

### 4. Create Template Tests

Create `tests/unit/ui/input/TemplateTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/input/GestureTemplate.hpp"
#include "ui/input/GestureTemplateManager.hpp"
#include "ui/input/GestureTemplateView.hpp"

using namespace void_contingency::ui;

TEST(GestureTemplateManagerTest, TemplateManagement) {
    auto& manager = GestureTemplateManager::getInstance();

    // Create test gesture
    RecordedGesture gesture;
    gesture.id = "test_gesture";
    gesture.type = GestureType::Swipe;
    gesture.points = {
        {glm::vec2(0.0f), 1.0f, std::chrono::system_clock::now()},
        {glm::vec2(100.0f, 0.0f), 1.0f, std::chrono::system_clock::now() + std::chrono::milliseconds(100)}
    };

    // Create template metadata
    TemplateMetadata metadata;
    metadata.name = "Test Template";
    metadata.description = "A test gesture template";
    metadata.category = TemplateCategory::Navigation;
    metadata.tags = {"swipe", "navigation"};
    metadata.authorId = "user1";

    // Create template
    std::string templateId = manager.createTemplate(gesture, metadata);

    // Set validation
    TemplateValidation validation;
    validation.minSpeed = 0.5f;
    validation.maxSpeed = 2.0f;
    validation.minPressure = 0.5f;
    validation.maxPressure = 1.0f;
    manager.setValidation(templateId, validation);

    // Verify template
    auto retrievedGesture = manager.getTemplate(templateId);
    EXPECT_EQ(retrievedGesture.id, gesture.id);

    auto retrievedMetadata = manager.getMetadata(templateId);
    EXPECT_EQ(retrievedMetadata.name, metadata.name);

    // Generate preview
    auto preview = manager.generatePreview(templateId);
    EXPECT_TRUE(preview.isValid);

    // Delete template
    manager.deleteTemplate(templateId);
}

TEST(GestureTemplateViewTest, ViewRendering) {
    GestureTemplateView view;

    // Set test data
    std::string templateId = "test_template";
    view.setTemplateId(templateId);

    TemplateMetadata metadata;
    metadata.templateId = templateId;
    metadata.name = "Test Template";
    metadata.description = "A test gesture template";
    metadata.category = TemplateCategory::Navigation;
    view.setMetadata(metadata);

    TemplateValidation validation;
    validation.minSpeed = 0.5f;
    validation.maxSpeed = 2.0f;
    view.setValidation(validation);

    // Verify view state
    EXPECT_TRUE(view.isEditable());
    view.setEditable(false);
    EXPECT_FALSE(view.isEditable());
}
```

## Next Steps

1. Add gesture training system
2. Implement gesture export/import
3. Add gesture collaboration features
4. Implement gesture recognition system

## Common Pitfalls

1. **Template Validation**: Handle edge cases properly
2. **Preview Generation**: Ensure accurate previews
3. **Version Management**: Maintain template versions
4. **Performance**: Optimize template matching

## Best Practices

1. Use efficient template matching
2. Implement proper validation
3. Support template versioning
4. Enable template sharing
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
