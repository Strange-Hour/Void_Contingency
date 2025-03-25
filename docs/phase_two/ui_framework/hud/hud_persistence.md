# Implementing HUD Persistence

## Navigation

- Previous: [HUD Styling Tutorial](./hud_styling.md)
- Next: [HUD Debugging Tutorial](./hud_debugging.md)

This tutorial will guide you through implementing a persistence system for the HUD in "Void Contingency", focusing on saving and loading configurations, style presets, and user preferences.

## Overview

The HUD persistence system will:

- Save and load HUD configurations
- Manage style presets
- Handle user preferences
- Support configuration versioning
- Enable configuration validation
- Provide backup and restore functionality

## Implementation Steps

### 1. Create HUD Persistence Types

Create `include/ui/hud/HUDPersistence.hpp`:

```cpp
#pragma once

#include "ui/hud/HUDTypes.hpp"
#include "ui/hud/HUDStyle.hpp"
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include <chrono>

namespace void_contingency {
namespace ui {

struct HUDConfig {
    std::string id;
    std::string name;
    std::string description;
    std::string version{"1.0.0"};
    std::chrono::system_clock::time_point lastModified;
    std::vector<std::string> enabledElements;
    std::vector<std::string> disabledElements;
    std::unordered_map<std::string, std::string> elementSettings;
    std::vector<std::string> stylePresets;
    std::unordered_map<std::string, std::string> userPreferences;
};

struct StylePreset {
    std::string id;
    std::string name;
    std::string description;
    HUDStyle style;
    std::vector<std::string> tags;
    bool isDefault{false};
    std::chrono::system_clock::time_point lastUsed;
};

struct UserPreferences {
    std::string userId;
    std::string theme{"dark"};
    float opacity{0.8f};
    float scale{1.0f};
    bool showTooltips{true};
    bool showDebugInfo{false};
    std::vector<std::string> favoritePresets;
    std::unordered_map<std::string, std::string> customSettings;
};

struct PersistenceOptions {
    bool autoSave{true};
    bool createBackups{true};
    int maxBackups{5};
    std::string saveDirectory{"config/hud"};
    std::string backupDirectory{"config/hud/backups"};
    bool compressFiles{true};
    bool encryptFiles{false};
};

using PersistenceCallback = std::function<void(const HUDConfig&)>;

} // namespace ui
} // namespace void_contingency
```

### 2. Create HUD Persistence Manager

Create `include/ui/hud/HUDPersistenceManager.hpp`:

```cpp
#pragma once

#include "ui/hud/HUDPersistence.hpp"
#include <memory>
#include <vector>
#include <unordered_map>

namespace void_contingency {
namespace ui {

class HUDPersistenceManager {
public:
    static HUDPersistenceManager& getInstance();

    // Configuration management
    void saveConfig(const HUDConfig& config);
    HUDConfig loadConfig(const std::string& id);
    void deleteConfig(const std::string& id);
    void backupConfig(const std::string& id);
    void restoreConfig(const std::string& id);

    // Style preset management
    void savePreset(const StylePreset& preset);
    StylePreset loadPreset(const std::string& id);
    void deletePreset(const std::string& id);
    std::vector<StylePreset> getAllPresets() const;

    // User preferences management
    void savePreferences(const UserPreferences& preferences);
    UserPreferences loadPreferences(const std::string& userId);
    void updatePreferences(const std::string& userId, const std::string& key, const std::string& value);

    // Persistence control
    void update(float deltaTime);
    void autoSave();
    void validateConfig(const HUDConfig& config);
    void cleanupOldBackups();

    // Persistence query
    bool hasConfig(const std::string& id) const;
    bool hasPreset(const std::string& id) const;
    bool hasPreferences(const std::string& userId) const;
    std::vector<std::string> getConfigVersions(const std::string& id) const;

    // Persistence subscription
    void subscribeToConfig(const std::string& id, PersistenceCallback callback);
    void unsubscribeFromConfig(const std::string& id, PersistenceCallback callback);

private:
    HUDPersistenceManager() = default;
    ~HUDPersistenceManager() = default;
    HUDPersistenceManager(const HUDPersistenceManager&) = delete;
    HUDPersistenceManager& operator=(const HUDPersistenceManager&) = delete;

    // Persistence state
    std::unordered_map<std::string, HUDConfig> configs_;
    std::unordered_map<std::string, StylePreset> presets_;
    std::unordered_map<std::string, UserPreferences> preferences_;
    std::unordered_map<std::string, std::vector<PersistenceCallback>> callbacks_;
    PersistenceOptions options_;

    // Helper methods
    void saveToFile(const std::string& path, const std::string& data);
    std::string loadFromFile(const std::string& path);
    void compressData(std::string& data);
    void decompressData(std::string& data);
    void encryptData(std::string& data);
    void decryptData(std::string& data);
    void notifyCallbacks(const std::string& id, const HUDConfig& config);
    bool validateConfigVersion(const HUDConfig& config);
    void createBackup(const std::string& id);
    void restoreFromBackup(const std::string& id);
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create HUD Persistence View

Create `include/ui/hud/HUDPersistenceView.hpp`:

```cpp
#pragma once

#include "ui/hud/HUDPersistence.hpp"
#include "ui/components/Component.hpp"
#include <memory>

namespace void_contingency {
namespace ui {

class HUDPersistenceView : public Component {
public:
    HUDPersistenceView();
    ~HUDPersistenceView() override;

    // View control
    void setConfig(const HUDConfig& config);
    void setPreset(const StylePreset& preset);
    void setPreferences(const UserPreferences& preferences);

    // Interactive features
    void setInteractive(bool interactive) { interactive_ = interactive; }
    bool isInteractive() const { return interactive_; }

    // View customization
    void setShowConfig(bool show) { showConfig_ = show; }
    void setShowPresets(bool show) { showPresets_ = show; }
    void setShowPreferences(bool show) { showPreferences_ = show; }

protected:
    void render(Renderer& renderer) override;
    void handleInput(const InputEvent& event) override;
    void update() override;

private:
    // View state
    HUDConfig currentConfig_;
    StylePreset currentPreset_;
    UserPreferences currentPreferences_;
    bool interactive_{false};
    bool showConfig_{true};
    bool showPresets_{true};
    bool showPreferences_{true};

    // Helper methods
    void renderConfig(Renderer& renderer);
    void renderPresets(Renderer& renderer);
    void renderPreferences(Renderer& renderer);
    void handlePersistenceInput(const InputEvent& event);
    void updateVisualization();
};

} // namespace ui
} // namespace void_contingency
```

### 4. Create Persistence Tests

Create `tests/unit/ui/hud/PersistenceTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/hud/HUDPersistence.hpp"
#include "ui/hud/HUDPersistenceManager.hpp"
#include "ui/hud/HUDPersistenceView.hpp"

using namespace void_contingency::ui;

TEST(HUDPersistenceManagerTest, ConfigManagement) {
    auto& manager = HUDPersistenceManager::getInstance();

    // Create test config
    HUDConfig config;
    config.id = "test_config";
    config.name = "Test Config";
    config.version = "1.0.0";
    config.enabledElements = {"health", "ammo"};
    config.disabledElements = {"debug"};
    config.elementSettings["health.position"] = "top-left";
    config.elementSettings["ammo.position"] = "top-right";

    // Save config
    manager.saveConfig(config);

    // Verify config
    EXPECT_TRUE(manager.hasConfig("test_config"));
    EXPECT_EQ(manager.loadConfig("test_config").name, "Test Config");

    // Update config
    config.version = "1.1.0";
    manager.saveConfig(config);
    EXPECT_EQ(manager.loadConfig("test_config").version, "1.1.0");

    // Delete config
    manager.deleteConfig("test_config");
    EXPECT_FALSE(manager.hasConfig("test_config"));
}

TEST(HUDPersistenceViewTest, ViewRendering) {
    HUDPersistenceView view;

    // Set test config
    HUDConfig config;
    config.id = "test_config";
    config.name = "Test Config";
    view.setConfig(config);

    // Set test preset
    StylePreset preset;
    preset.id = "test_preset";
    preset.name = "Test Preset";
    view.setPreset(preset);

    // Verify view state
    EXPECT_FALSE(view.isInteractive());
    view.setInteractive(true);
    EXPECT_TRUE(view.isInteractive());
}
```

## Next Steps

1. Add HUD debugging tools
2. Implement HUD analytics
3. Add HUD optimization
4. Implement HUD sharing features

## Common Pitfalls

1. **Version Compatibility**: Handle config version changes
2. **Data Validation**: Validate saved configurations
3. **Performance**: Optimize save/load operations
4. **Resource Usage**: Control memory usage for backups

## Best Practices

1. Use efficient serialization
2. Implement proper validation
3. Support compression
4. Enable encryption
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
- [HUD Debugging Tutorial](./hud_debugging.md)
- [HUD Analytics Tutorial](./hud_analytics.md)
- [HUD Optimization Tutorial](./hud_optimization.md)
- [HUD Sharing Tutorial](./hud_sharing.md)
- [HUD Testing Tutorial](./hud_testing.md)
- [HUD Profiling Tutorial](./hud_profiling.md)
- [HUD Benchmarking Tutorial](./hud_benchmarking.md)
