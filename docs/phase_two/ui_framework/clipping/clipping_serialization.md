# Implementing Clipping Serialization

## Navigation

- Previous: [Clipping Debugging Tutorial](./clipping_debugging.md)
- Next: [Clipping Compression Tutorial](./clipping_compression.md)

This tutorial will guide you through implementing serialization for the clipping system in "Void Contingency", focusing on efficient state persistence and version management.

## Overview

The serialization system will:

- Support binary serialization
- Enable JSON serialization
- Handle version management
- Provide state persistence
- Enable state validation

## Implementation Steps

### 1. Create Clipping Serializer

Create `include/ui/serialize/ClipSerializer.hpp`:

```cpp
#pragma once

#include "ui/clip/ClipCache.hpp"
#include <vector>
#include <memory>
#include <string>
#include <nlohmann/json.hpp>

namespace void_contingency {
namespace ui {

struct SerializationVersion {
    uint32_t major{1};
    uint32_t minor{0};
    uint32_t patch{0};

    bool operator==(const SerializationVersion& other) const {
        return major == other.major &&
               minor == other.minor &&
               patch == other.patch;
    }
};

class ClipSerializer {
public:
    static ClipSerializer& getInstance();

    // Binary serialization
    void serializeBinary(const NestedClipRect& rect, std::ostream& stream);
    void deserializeBinary(NestedClipRect& rect, std::istream& stream);

    // JSON serialization
    void serializeJson(const NestedClipRect& rect, nlohmann::json& json);
    void deserializeJson(NestedClipRect& rect, const nlohmann::json& json);

    // Version management
    void setVersion(const SerializationVersion& version);
    const SerializationVersion& getVersion() const { return version_; }
    bool isCompatible(const SerializationVersion& other) const;

    // State persistence
    bool saveToFile(const std::string& filename, const NestedClipRect& rect);
    bool loadFromFile(const std::string& filename, NestedClipRect& rect);

    // State validation
    bool validateState(const NestedClipRect& rect) const;
    std::string getLastError() const { return lastError_; }

private:
    ClipSerializer() = default;
    ~ClipSerializer() = default;
    ClipSerializer(const ClipSerializer&) = delete;
    ClipSerializer& operator=(const ClipSerializer&) = delete;

    // Serialization state
    SerializationVersion version_;
    std::string lastError_;

    // Binary format
    struct BinaryHeader {
        char magic[4];
        SerializationVersion version;
        uint32_t dataSize;
    };

    // Helper methods
    void writeHeader(std::ostream& stream, uint32_t dataSize);
    void readHeader(std::istream& stream, uint32_t& dataSize);
    void writeTransform(std::ostream& stream, const glm::mat4& transform);
    void readTransform(std::istream& stream, glm::mat4& transform);
    void writeVec2(std::ostream& stream, const glm::vec2& vec);
    void readVec2(std::istream& stream, glm::vec2& vec);
};

} // namespace ui
} // namespace void_contingency
```

### 2. Create Clipping State Manager

Create `include/ui/serialize/ClipStateManager.hpp`:

```cpp
#pragma once

#include "ui/serialize/ClipSerializer.hpp"
#include "ui/clip/ClipCache.hpp"
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

namespace void_contingency {
namespace ui {

struct ClipState {
    std::string name;
    NestedClipRect root;
    SerializationVersion version;
    std::chrono::system_clock::time_point timestamp;
};

class ClipStateManager {
public:
    static ClipStateManager& getInstance();

    // State management
    void saveState(const std::string& name, const NestedClipRect& root);
    bool loadState(const std::string& name, NestedClipRect& root);
    void deleteState(const std::string& name);

    // State listing
    std::vector<std::string> getStateNames() const;
    const ClipState* getState(const std::string& name) const;

    // State validation
    bool validateState(const std::string& name) const;
    std::vector<std::string> getValidationErrors(const std::string& name) const;

    // State persistence
    bool saveStatesToFile(const std::string& filename);
    bool loadStatesFromFile(const std::string& filename);

private:
    ClipStateManager() = default;
    ~ClipStateManager() = default;
    ClipStateManager(const ClipStateManager&) = delete;
    ClipStateManager& operator=(const ClipStateManager&) = delete;

    // State storage
    std::unordered_map<std::string, ClipState> states_;
    std::vector<std::string> validationErrors_;

    // Helper methods
    void updateStateTimestamp(ClipState& state);
    bool validateStateInternal(const ClipState& state);
    void clearValidationErrors();
};

} // namespace ui
} // namespace void_contingency
```

### 3. Create Serialization Tests

Create `tests/unit/ui/serialize/SerializationTest.cpp`:

```cpp
#include <gtest/gtest.h>
#include "ui/serialize/ClipSerializer.hpp"
#include "ui/serialize/ClipStateManager.hpp"
#include "ui/components/ClippingTransform.hpp"

using namespace void_contingency::ui;

TEST(ClipSerializerTest, BinarySerialization) {
    auto& serializer = ClipSerializer::getInstance();

    // Create test clip rect
    NestedClipRect rect;
    rect.position = glm::vec2(10.0f, 20.0f);
    rect.size = glm::vec2(100.0f, 200.0f);
    rect.transform = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 0.0f));

    // Serialize to binary
    std::stringstream stream;
    serializer.serializeBinary(rect, stream);

    // Deserialize from binary
    NestedClipRect loadedRect;
    serializer.deserializeBinary(loadedRect, stream);

    // Verify data
    EXPECT_EQ(loadedRect.position, rect.position);
    EXPECT_EQ(loadedRect.size, rect.size);
    EXPECT_EQ(loadedRect.transform, rect.transform);
}

TEST(ClipSerializerTest, JsonSerialization) {
    auto& serializer = ClipSerializer::getInstance();

    // Create test clip rect
    NestedClipRect rect;
    rect.position = glm::vec2(10.0f, 20.0f);
    rect.size = glm::vec2(100.0f, 200.0f);

    // Serialize to JSON
    nlohmann::json json;
    serializer.serializeJson(rect, json);

    // Deserialize from JSON
    NestedClipRect loadedRect;
    serializer.deserializeJson(loadedRect, json);

    // Verify data
    EXPECT_EQ(loadedRect.position, rect.position);
    EXPECT_EQ(loadedRect.size, rect.size);
}

TEST(ClipStateManagerTest, StateManagement) {
    auto& manager = ClipStateManager::getInstance();

    // Create test state
    NestedClipRect root;
    root.position = glm::vec2(0.0f, 0.0f);
    root.size = glm::vec2(100.0f, 100.0f);

    // Save state
    manager.saveState("TestState", root);

    // Load state
    NestedClipRect loadedRoot;
    EXPECT_TRUE(manager.loadState("TestState", loadedRoot));

    // Verify state
    EXPECT_EQ(loadedRoot.position, root.position);
    EXPECT_EQ(loadedRoot.size, root.size);

    // Delete state
    manager.deleteState("TestState");
    EXPECT_EQ(manager.getState("TestState"), nullptr);
}

TEST(ClipStateManagerTest, StateValidation) {
    auto& manager = ClipStateManager::getInstance();

    // Create test state
    NestedClipRect root;
    root.position = glm::vec2(0.0f, 0.0f);
    root.size = glm::vec2(100.0f, 100.0f);

    // Save state
    manager.saveState("TestState", root);

    // Validate state
    EXPECT_TRUE(manager.validateState("TestState"));

    // Get validation errors
    auto errors = manager.getValidationErrors("TestState");
    EXPECT_TRUE(errors.empty());
}
```

## Next Steps

1. Implement clipping region compression
2. Add clipping region profiling
3. Implement clipping region validation
4. Add clipping region migration

## Common Pitfalls

1. **Version Compatibility**: Handle version mismatches
2. **Data Integrity**: Ensure data validation
3. **Performance**: Optimize serialization
4. **Memory Usage**: Manage state storage

## Best Practices

1. Use efficient serialization formats
2. Implement version management
3. Validate serialized data
4. Support state persistence
5. Enable error reporting

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
- [Clipping Compression Tutorial](./clipping_compression.md)
