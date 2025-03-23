# Implementing Upgrade System

## Navigation

- Previous: [Combat Framework Tutorial](./combat_framework.md)
- Next: [UI Component System Tutorial](../ui_framework/core/ui_component_system.md)

## What We're Building

We're implementing a comprehensive upgrade system that allows players to enhance their ships, weapons, and abilities throughout the game. This system manages upgrade paths, requirements, costs, and effects, providing meaningful progression and customization options.

## Game Development Concepts Covered

1. **Upgrade Architecture**:

   - Upgrade trees
   - Dependency management
   - Cost systems
   - Effect application
   - Persistence

2. **Progression Systems**:

   - Experience points
   - Level requirements
   - Resource costs
   - Unlock conditions
   - Progression curves

3. **Game Balance**:
   - Power scaling
   - Resource economy
   - Upgrade paths
   - Player choice
   - Replayability

## Why This Implementation?

1. **Flexibility**: Support for various upgrade types and paths
2. **Balance**: Easy to adjust costs and effects
3. **Extensibility**: Simple to add new upgrades
4. **Persistence**: Reliable upgrade state saving
5. **Feedback**: Clear upgrade effects and requirements
6. **Performance**: Efficient upgrade processing
7. **Maintainability**: Clean upgrade definition system

## Implementation Steps

### 1. Create Upgrade Types

Create `include/game/upgrade/UpgradeTypes.hpp`:

```cpp
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace void_contingency {
namespace game {

enum class UpgradeType {
    Ship,
    Weapon,
    Shield,
    Engine,
    Power,
    Special
};

enum class UpgradeEffect {
    StatBoost,
    NewAbility,
    Enhancement,
    Modification,
    Unlock
};

struct UpgradeCost {
    std::string resourceType;
    float amount;
    int levelRequired;
};

struct UpgradeRequirement {
    std::vector<std::string> prerequisiteUpgrades;
    int playerLevel;
    std::unordered_map<std::string, float> resourceRequirements;
    std::vector<std::string> achievementRequirements;
};

struct UpgradeDefinition {
    std::string id;
    std::string name;
    std::string description;
    UpgradeType type;
    UpgradeEffect effect;
    int maxLevel;
    std::vector<UpgradeCost> costs;
    UpgradeRequirement requirements;
    std::unordered_map<std::string, float> statModifiers;
    std::vector<std::string> unlockedAbilities;
};

} // namespace game
} // namespace void_contingency
```

### 2. Create Upgrade Manager

Create `include/game/upgrade/UpgradeManager.hpp`:

```cpp
#pragma once

#include "game/upgrade/UpgradeTypes.hpp"
#include "game/entity/Entity.hpp"
#include <vector>
#include <memory>
#include <unordered_map>

namespace void_contingency {
namespace game {

class UpgradeManager {
public:
    static UpgradeManager& getInstance();

    void initialize();
    void shutdown();

    // Upgrade registration
    void registerUpgrade(const UpgradeDefinition& upgrade);
    void unregisterUpgrade(const std::string& upgradeId);
    bool hasUpgrade(const std::string& upgradeId) const;

    // Upgrade operations
    bool canApplyUpgrade(EntityPtr entity, const std::string& upgradeId) const;
    bool applyUpgrade(EntityPtr entity, const std::string& upgradeId);
    bool removeUpgrade(EntityPtr entity, const std::string& upgradeId);
    int getUpgradeLevel(EntityPtr entity, const std::string& upgradeId) const;

    // Upgrade queries
    std::vector<UpgradeDefinition> getAvailableUpgrades(EntityPtr entity) const;
    std::vector<UpgradeDefinition> getAppliedUpgrades(EntityPtr entity) const;
    std::vector<UpgradeDefinition> getUpgradesByType(UpgradeType type) const;
    const UpgradeDefinition* getUpgradeDefinition(const std::string& upgradeId) const;

    // Upgrade path
    std::vector<std::string> getUpgradePath(const std::string& upgradeId) const;
    std::vector<std::string> getUnlockedUpgrades(const std::string& upgradeId) const;
    bool isUpgradePathComplete(EntityPtr entity, const std::string& pathId) const;

    // Upgrade effects
    void applyUpgradeEffects(EntityPtr entity);
    void removeUpgradeEffects(EntityPtr entity);
    std::unordered_map<std::string, float> getUpgradeModifiers(EntityPtr entity) const;

    // Persistence
    void saveUpgradeState(EntityPtr entity, const std::string& filename);
    void loadUpgradeState(EntityPtr entity, const std::string& filename);

private:
    UpgradeManager() = default;
    ~UpgradeManager() = default;
    UpgradeManager(const UpgradeManager&) = delete;
    UpgradeManager& operator=(const UpgradeManager&) = delete;

    struct UpgradeState {
        std::unordered_map<std::string, int> upgradeLevels;
        std::unordered_map<std::string, std::vector<std::string>> unlockedAbilities;
        std::unordered_map<std::string, float> activeModifiers;
    };

    bool validateUpgradeRequirements(EntityPtr entity, const UpgradeDefinition& upgrade) const;
    bool validateResourceRequirements(EntityPtr entity, const UpgradeCost& cost) const;
    void applyUpgradeModifiers(EntityPtr entity, const UpgradeDefinition& upgrade);
    void removeUpgradeModifiers(EntityPtr entity, const UpgradeDefinition& upgrade);
    void notifyUpgradeApplied(EntityPtr entity, const UpgradeDefinition& upgrade);

    std::unordered_map<std::string, UpgradeDefinition> upgrades_;
    std::unordered_map<EntityPtr, UpgradeState> entityUpgrades_;
};

} // namespace game
} // namespace void_contingency
```

## Key Concepts Explained

### Upgrade Architecture

1. **Upgrade Types**:

   - Ship upgrades
   - Weapon upgrades
   - Shield upgrades
   - Engine upgrades
   - Power upgrades
   - Special upgrades

2. **Upgrade Effects**:

   - Stat boosts
   - New abilities
   - Enhancements
   - Modifications
   - Unlocks

3. **Upgrade Requirements**:
   - Prerequisites
   - Level requirements
   - Resource costs
   - Achievement gates

### Upgrade Management

1. **State Tracking**:

   - Upgrade levels
   - Applied upgrades
   - Unlocked abilities
   - Active modifiers

2. **Path Management**:

   - Upgrade trees
   - Dependencies
   - Progression paths
   - Completion tracking

3. **Effect Processing**:
   - Modifier application
   - Ability unlocking
   - Stat calculation
   - Effect stacking

### Upgrade Persistence

1. **Save System**:

   - Upgrade state
   - Progress tracking
   - Resource tracking
   - Achievement state

2. **Load System**:
   - State restoration
   - Modifier reapplication
   - Validation checks
   - Error handling

## Common Pitfalls

1. **Balance Issues**:

   - Power creep
   - Resource economy
   - Progression pacing
   - Choice impact
   - Path viability

2. **Technical Issues**:

   - State corruption
   - Effect stacking
   - Save/load errors
   - Memory leaks
   - Performance impact

3. **Design Issues**:

   - Complex paths
   - Unclear effects
   - Poor feedback
   - Limited choice
   - Dead ends

4. **User Experience**:
   - Confusing UI
   - Poor feedback
   - Unclear costs
   - Hidden requirements
   - Frustrating gates

## Best Practices

1. **Design Principles**:

   - Clear progression
   - Meaningful choices
   - Balanced paths
   - Fair costs
   - Good feedback

2. **Technical Implementation**:

   - Clean architecture
   - Efficient processing
   - Safe persistence
   - Error handling
   - Performance optimization

3. **User Experience**:

   - Clear UI
   - Good feedback
   - Visual effects
   - Sound effects
   - Tooltips

4. **Balance**:
   - Regular testing
   - Data analysis
   - Player feedback
   - Cost adjustment
   - Effect tuning

## Testing

Create test file `tests/unit/game/upgrade/UpgradeManagerTest.cpp`:

```cpp
TEST(UpgradeManagerTest, BasicUpgrade) {
    auto& manager = UpgradeManager::getInstance();

    // Create entity and upgrade
    auto entity = std::make_shared<Entity>("TestEntity");
    UpgradeDefinition upgrade{
        "test_upgrade",
        "Test Upgrade",
        "A test upgrade",
        UpgradeType::Ship,
        UpgradeEffect::StatBoost,
        3,
        {{{"credits", 100.0f, 1}}},
        {},
        {{"health", 1.5f}},
        {}
    };

    // Register and apply upgrade
    manager.registerUpgrade(upgrade);
    EXPECT_TRUE(manager.hasUpgrade("test_upgrade"));
    EXPECT_TRUE(manager.canApplyUpgrade(entity, "test_upgrade"));
    EXPECT_TRUE(manager.applyUpgrade(entity, "test_upgrade"));

    // Check upgrade state
    EXPECT_EQ(manager.getUpgradeLevel(entity, "test_upgrade"), 1);
    auto modifiers = manager.getUpgradeModifiers(entity);
    EXPECT_FLOAT_EQ(modifiers["health"], 1.5f);
}

TEST(UpgradeManagerTest, UpgradePath) {
    auto& manager = UpgradeManager::getInstance();
    auto entity = std::make_shared<Entity>("TestEntity");

    // Create upgrade path
    UpgradeDefinition basic{
        "basic_upgrade",
        "Basic Upgrade",
        "A basic upgrade",
        UpgradeType::Ship,
        UpgradeEffect::StatBoost,
        1,
        {{{"credits", 50.0f, 1}}},
        {},
        {{"health", 1.2f}},
        {}
    };

    UpgradeDefinition advanced{
        "advanced_upgrade",
        "Advanced Upgrade",
        "An advanced upgrade",
        UpgradeType::Ship,
        UpgradeEffect::Enhancement,
        1,
        {{{"credits", 100.0f, 2}}},
        {{"basic_upgrade"}, 2, {}, {}},
        {{"health", 1.5f}},
        {}
    };

    // Register upgrades
    manager.registerUpgrade(basic);
    manager.registerUpgrade(advanced);

    // Test path requirements
    EXPECT_TRUE(manager.canApplyUpgrade(entity, "basic_upgrade"));
    EXPECT_FALSE(manager.canApplyUpgrade(entity, "advanced_upgrade"));

    // Apply basic upgrade
    EXPECT_TRUE(manager.applyUpgrade(entity, "basic_upgrade"));
    EXPECT_TRUE(manager.hasUpgrade("basic_upgrade"));

    // Check path completion
    auto path = manager.getUpgradePath("advanced_upgrade");
    EXPECT_EQ(path.size(), 2);
    EXPECT_EQ(path[0], "basic_upgrade");
    EXPECT_EQ(path[1], "advanced_upgrade");
}

TEST(UpgradeManagerTest, Persistence) {
    auto& manager = UpgradeManager::getInstance();
    auto entity = std::make_shared<Entity>("TestEntity");

    // Apply some upgrades
    UpgradeDefinition upgrade{
        "test_upgrade",
        "Test Upgrade",
        "A test upgrade",
        UpgradeType::Ship,
        UpgradeEffect::StatBoost,
        3,
        {{{"credits", 100.0f, 1}}},
        {},
        {{"health", 1.5f}},
        {}
    };

    manager.registerUpgrade(upgrade);
    manager.applyUpgrade(entity, "test_upgrade");

    // Save state
    manager.saveUpgradeState(entity, "test_save.json");

    // Clear state
    manager.removeUpgrade(entity, "test_upgrade");
    EXPECT_EQ(manager.getUpgradeLevel(entity, "test_upgrade"), 0);

    // Load state
    manager.loadUpgradeState(entity, "test_save.json");
    EXPECT_EQ(manager.getUpgradeLevel(entity, "test_upgrade"), 1);
    auto modifiers = manager.getUpgradeModifiers(entity);
    EXPECT_FLOAT_EQ(modifiers["health"], 1.5f);
}
```

## Next Steps

1. Add advanced features:

   - Upgrade trees visualization
   - Cost calculator
   - Path optimizer
   - Effect preview

2. Implement upgrade tools:

   - Tree editor
   - Balance analyzer
   - Cost adjuster
   - Effect tester

3. Create upgrade systems:

   - Specialization paths
   - Hybrid upgrades
   - Dynamic costs
   - Temporary upgrades

4. Add optimization features:
   - Effect caching
   - Batch processing
   - State compression
   - Load optimization

## Additional Resources

- [Combat Framework Tutorial](./combat_framework.md)
- [Resource Management Tutorial](./resource_management.md)
