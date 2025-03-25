# Implementing Menu Persistence

## Navigation

- Previous: [Menu Input Handling Tutorial](./menu_input_handling.md)
- Next: [Menu Debugging Tutorial](./menu_debugging.md)

This tutorial will guide you through implementing menu persistence for "Void Contingency", providing a robust system for saving and loading menu states, configurations, and settings.

## Overview

The menu persistence system will:

- Save menu states
- Load menu configurations
- Manage menu settings
- Handle menu data serialization
- Enable menu data compression
- Provide menu data validation

## Implementation Steps

### 1. Create Menu Persistence Types

Create `include/ui/menu/MenuPersistence.hpp`:

```cpp
#pragma once

#include "ui/menu/MenuComponents.hpp"
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include <variant>

namespace void_contingency {
namespace ui {

enum class PersistenceType {
    State,
    Configuration,
    Settings,
    Layout,
    Style,
    Custom
};

enum class PersistenceFormat {
    Binary,
    JSON,
    XML,
    YAML,
    Custom
};

enum class PersistenceMode {
    Read,
    Write,
    Append,
    Overwrite,
    Custom
};

struct PersistenceOptions {
    PersistenceType type{PersistenceType::State};
    PersistenceFormat format{PersistenceFormat::Binary};
    PersistenceMode mode{PersistenceMode::Write};
    bool compress{false};
    bool encrypt{false};
    bool validate{true};
    std::string path;
    std::string filename;
    std::string extension;
    std::unordered_map<std::string, std::variant<bool, int, float, std::string>> properties;
};

struct PersistenceData {
    std::string id;
    std::string version;
    std::string timestamp;
    std::string checksum;
    std::vector<uint8_t> data;
    std::unordered_map<std::string, std::variant<bool, int, float, std::string>> metadata;
};

class MenuPersistence {
public:
    MenuPersistence();
    ~MenuPersistence();

    // Persistence identification
    void setId(const std::string& id) { properties_.id = id; }
    const std::string& getId() const { return properties_.id; }

    // Persistence properties
    void setType(PersistenceType type) { properties_.type = type; }
    PersistenceType getType() const { return properties_.type; }

    void setFormat(PersistenceFormat format) { properties_.format = format; }
    PersistenceFormat getFormat() const { return properties_.format; }

    void setMode(PersistenceMode mode) { properties_.mode = mode; }
    PersistenceMode getMode() const { return properties_.mode; }

    // Persistence control
    void save(const PersistenceData& data);
    void load(PersistenceData& data);
    void clear();
    void reset();

    // Persistence validation
    bool validate(const PersistenceData& data) const;
    bool verify(const PersistenceData& data) const;
    bool check(const PersistenceData& data) const;

    // Persistence events
    void setOnSave(std::function<void(const PersistenceData&)> callback) { onSave_ = callback; }
    void setOnLoad(std::function<void(const PersistenceData&)> callback) { onLoad_ = callback; }
    void setOnValidate(std::function<void(const PersistenceData&)> callback) { onValidate_ = callback; }

    // Persistence update
    void update(float deltaTime);

private:
    struct PersistenceProperties {
        std::string id;
        PersistenceType type{PersistenceType::State};
        PersistenceFormat format{PersistenceFormat::Binary};
        PersistenceMode mode{PersistenceMode::Write};
        bool compress{false};
        bool encrypt{false};
        bool validate{true};
        std::string path;
        std::string filename;
        std::string extension;
        std::unordered_map<std::string, std::variant<bool, int, float, std::string>> properties;
    };

    PersistenceProperties properties_;
    std::vector<PersistenceData> dataCache_;
    bool isSaving_{false};
    bool isLoading_{false};
    bool isValidating_{false};

    // Persistence callbacks
    std::function<void(const PersistenceData&)> onSave_;
    std::function<void(const PersistenceData&)> onLoad_;
    std::function<void(const PersistenceData&)> onValidate_;

    // Helper methods
    void processData(const PersistenceData& data);
    void handleValidation(const PersistenceData& data);
    void updateCache();
    void validateData();
    void backupData();
    void restoreData();
};

} // namespace ui
} // namespace void_contingency
```

### 2. Create Menu Persistence Manager

Create `include/ui/menu/MenuPersistenceManager.hpp`:

```cpp
#pragma once

#include "ui/menu/MenuPersistence.hpp"
#include <memory>
#include <vector>
#include <unordered_map>
#include <queue>
#include <functional>

namespace void_contingency {
namespace ui {

class MenuPersistenceManager {
public:
    static MenuPersistenceManager& getInstance();

    // Persistence management
    void addPersistence(std::shared_ptr<MenuPersistence> persistence);
    void removePersistence(const std::string& id);
    void updatePersistence(const std::string& id, std::shared_ptr<MenuPersistence> persistence);
    std::shared_ptr<MenuPersistence> getPersistence(const std::string& id) const;
    std::vector<std::shared_ptr<MenuPersistence>> getAllPersistences() const;

    // Persistence control
    void savePersistence(const std::string& id, const PersistenceData& data);
    void loadPersistence(const std::string& id, PersistenceData& data);
    void clearPersistence(const std::string& id);
    void resetPersistence(const std::string& id);

    // Persistence validation
    bool validatePersistence(const std::string& id, const PersistenceData& data) const;
    bool verifyPersistence(const std::string& id, const PersistenceData& data) const;
    bool checkPersistence(const std::string& id, const PersistenceData& data) const;

    // Persistence events
    void setOnPersistenceSave(std::function<void(const std::string&, const PersistenceData&)> callback);
    void setOnPersistenceLoad(std::function<void(const std::string&, const PersistenceData&)> callback);
    void setOnPersistenceValidate(std::function<void(const std::string&, const PersistenceData&)> callback);

    // Persistence debugging
    void enableDebugging(bool enable);
    void setDebugLevel(int level);
    void logPersistence(const std::string& message);

    // Persistence cleanup
    void cleanup();
    void reset();
    void clear();

private:
    MenuPersistenceManager() = default;
    ~MenuPersistenceManager() = default;
    MenuPersistenceManager(const MenuPersistenceManager&) = delete;
    MenuPersistenceManager& operator=(const MenuPersistenceManager&) = delete;

    // Persistence storage
    std::unordered_map<std::string, std::shared_ptr<MenuPersistence>> persistences_;
    std::queue<PersistenceData> dataQueue_;
    std::vector<std::string> activePersistences_;
    bool debuggingEnabled_{false};
    int debugLevel_{0};

    // Persistence callbacks
    std::function<void(const std::string&, const PersistenceData&)> onPersistenceSave_;
    std::function<void(const std::string&, const PersistenceData&)> onPersistenceLoad_;
    std::function<void(const std::string&, const PersistenceData&)> onPersistenceValidate_;

    // Helper methods
    void updatePersistences(float deltaTime);
    void handlePersistenceEvents();
    void processDataQueue();
    void validatePersistences();
    void cleanupPersistenceResources();
    void backupPersistenceState();
    void restorePersistenceState();
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create Menu Persistence View

Create `include/ui/menu/MenuPersistenceView.hpp`:

```cpp
#pragma once

#include "ui/menu/MenuPersistence.hpp"
#include "ui/components/Component.hpp"
#include <memory>

namespace void_contingency {
namespace ui {

class MenuPersistenceView : public Component {
public:
    MenuPersistenceView();
    ~MenuPersistenceView() override;

    // View control
    void setPersistence(std::shared_ptr<MenuPersistence> persistence);
    void setPersistences(const std::vector<std::shared_ptr<MenuPersistence>>& persistences);

    // Interactive features
    void setInteractive(bool interactive) { interactive_ = interactive; }
    bool isInteractive() const { return interactive_; }

    // View customization
    void setShowData(bool show) { showData_ = show; }
    void setShowMetadata(bool show) { showMetadata_ = show; }
    void setShowValidation(bool show) { showValidation_ = show; }
    void setShowControls(bool show) { showControls_ = show; }

protected:
    void render(Renderer& renderer) override;
    void handleInput(const InputEvent& event) override;
    void update() override;

private:
    // View state
    std::shared_ptr<MenuPersistence> persistence_;
    std::vector<std::shared_ptr<MenuPersistence>> persistences_;
    bool interactive_{false};
    bool showData_{true};
    bool showMetadata_{true};
    bool showValidation_{true};
    bool showControls_{true};

    // Helper methods
    void renderData(Renderer& renderer);
    void renderMetadata(Renderer& renderer);
    void renderValidation(Renderer& renderer);
    void renderControls(Renderer& renderer);
    void handlePersistenceInput(const InputEvent& event);
    void updateVisualization();
};

} // namespace ui
} // namespace void_contingency
```

### 4. Create Menu Persistence Tests

Create `tests/unit/ui/menu/PersistenceTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/menu/MenuPersistence.hpp"
#include "ui/menu/MenuPersistenceManager.hpp"
#include "ui/menu/MenuPersistenceView.hpp"

using namespace void_contingency::ui;

TEST(MenuPersistenceTest, PersistenceCreation) {
    auto persistence = std::make_shared<MenuPersistence>();
    persistence->setId("test_persistence");
    persistence->setType(PersistenceType::State);
    persistence->setFormat(PersistenceFormat::Binary);

    EXPECT_EQ(persistence->getId(), "test_persistence");
    EXPECT_EQ(persistence->getType(), PersistenceType::State);
    EXPECT_EQ(persistence->getFormat(), PersistenceFormat::Binary);
}

TEST(MenuPersistenceTest, PersistenceData) {
    auto persistence = std::make_shared<MenuPersistence>();
    persistence->setId("test_persistence");

    PersistenceData data;
    data.id = "test_data";
    data.version = "1.0";
    data.timestamp = "2024-01-01T00:00:00Z";
    data.checksum = "abc123";

    persistence->save(data);

    PersistenceData loadedData;
    persistence->load(loadedData);

    EXPECT_EQ(loadedData.id, "test_data");
    EXPECT_EQ(loadedData.version, "1.0");
    EXPECT_EQ(loadedData.timestamp, "2024-01-01T00:00:00Z");
    EXPECT_EQ(loadedData.checksum, "abc123");
}

TEST(MenuPersistenceManagerTest, PersistenceManagement) {
    auto& manager = MenuPersistenceManager::getInstance();

    // Create test persistence
    auto persistence = std::make_shared<MenuPersistence>();
    persistence->setId("test_persistence");
    persistence->setType(PersistenceType::State);

    // Add persistence
    manager.addPersistence(persistence);

    // Verify persistence
    auto retrievedPersistence = manager.getPersistence("test_persistence");
    EXPECT_EQ(retrievedPersistence->getId(), "test_persistence");
    EXPECT_EQ(retrievedPersistence->getType(), PersistenceType::State);
}

TEST(MenuPersistenceViewTest, ViewRendering) {
    MenuPersistenceView view;

    // Create test persistence
    auto persistence = std::make_shared<MenuPersistence>();
    persistence->setId("test_persistence");
    persistence->setType(PersistenceType::State);

    // Set persistence
    view.setPersistence(persistence);

    // Verify view state
    EXPECT_FALSE(view.isInteractive());
    view.setInteractive(true);
    EXPECT_TRUE(view.isInteractive());
}
```

## Next Steps

1. Add menu debugging
2. Implement menu analytics
3. Add menu optimization
4. Implement menu sharing

## Common Pitfalls

1. **Data Validation**: Validate persistence data correctly
2. **Data Format**: Handle data formats properly
3. **Data Compression**: Manage data compression correctly
4. **Data State**: Handle data state properly

## Best Practices

1. Use proper persistence types
2. Implement data validation
3. Handle data formats
4. Manage data state
5. Support customization

## Additional Resources

- [Menu Components Tutorial](./menu_components.md)
- [Menu Navigation Tutorial](./menu_navigation.md)
- [Menu State Management Tutorial](./menu_state_management.md)
- [Menu Transitions Tutorial](./menu_transitions.md)
- [Menu Input Handling Tutorial](./menu_input_handling.md)
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
- [HUD Benchmarking Tutorial](./hud_benchmarking.md)
- [Menu Debugging Tutorial](./menu_debugging.md)
- [Menu Analytics Tutorial](./menu_analytics.md)
- [Menu Optimization Tutorial](./menu_optimization.md)
- [Menu Sharing Tutorial](./menu_sharing.md)
