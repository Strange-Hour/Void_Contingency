# Implementing Resource Management

## Navigation

- Previous: [Exploration Mechanics Tutorial](./exploration.md)
- Next: [Combat Framework Tutorial](./combat_framework.md)

## What We're Building

We're implementing a comprehensive resource management system that handles all in-game resources, from basic materials to complex crafting components. This system provides a robust foundation for resource gathering, storage, trading, and crafting mechanics, enabling deep gameplay systems and meaningful player progression.

## Game Development Concepts Covered

1. **Resource Types**: Understanding different resource categories and properties
2. **Resource Storage**: Managing resource inventories and capacities
3. **Resource Flow**: Handling resource gathering and consumption
4. **Resource Economy**: Implementing trading and value systems
5. **Resource Crafting**: Creating complex resources from components
6. **Resource Persistence**: Saving and loading resource states
7. **Resource UI**: Displaying resource information
8. **Resource Events**: Handling resource-related events

## Why This Implementation?

1. **Flexibility**: Support for various resource types and properties
2. **Balance**: Easy to adjust resource values and rates
3. **Performance**: Efficient resource tracking and updates
4. **Extensibility**: Simple to add new resource types
5. **Reliability**: Robust resource transaction handling
6. **Debuggability**: Clear resource flow tracking
7. **Maintainability**: Clean separation of resource systems

## Implementation Steps

### 1. Create Resource Types

Create `include/game/resource/ResourceType.hpp`:

```cpp
#pragma once

#include <string>

namespace void_contingency {
namespace game {

enum class ResourceType {
    Credits,
    Energy,
    Minerals,
    RareElements,
    TechComponents,
    Ammunition,
    Fuel,
    RepairKits,
    ShieldCells,
    WeaponParts
};

struct ResourceProperties {
    std::string name;
    std::string description;
    float baseValue;
    float maxStorage;
    bool isStackable;
    bool isConsumable;
    bool isCraftable;
    bool isTradeble;
};

class ResourceTypeManager {
public:
    static ResourceTypeManager& getInstance();

    void registerType(ResourceType type, const ResourceProperties& props);
    const ResourceProperties& getProperties(ResourceType type) const;
    bool isRegistered(ResourceType type) const;
};

} // namespace game
} // namespace void_contingency
```

### 2. Create Resource Manager

Create `include/game/resource/ResourceManager.hpp`:

```cpp
#pragma once

#include "game/resource/ResourceType.hpp"
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

namespace void_contingency {
namespace game {

struct ResourceTransaction {
    enum class Type {
        Gather,
        Consume,
        Trade,
        Craft,
        Store,
        Retrieve
    };

    Type type;
    ResourceType resourceType;
    float amount;
    std::string source;
    std::string destination;
    float price;
    std::chrono::steady_clock::time_point timestamp;
};

class ResourceManager {
public:
    static ResourceManager& getInstance();

    void initialize();
    void shutdown();

    // Resource tracking
    float getResourceAmount(ResourceType type) const;
    void addResource(ResourceType type, float amount);
    bool consumeResource(ResourceType type, float amount);
    bool hasEnoughResources(ResourceType type, float amount) const;
    float getMaxStorage(ResourceType type) const;
    float getUsedStorage(ResourceType type) const;

    // Resource gathering
    void startGathering(ResourceType type, const std::string& source);
    void stopGathering(ResourceType type);
    float getGatheringRate(ResourceType type) const;
    bool isGathering(ResourceType type) const;

    // Resource trading
    bool initiateTrade(const std::string& traderId, ResourceType type, float amount, float price);
    void completeTrade(const std::string& tradeId);
    void cancelTrade(const std::string& tradeId);
    std::vector<ResourceTransaction> getActiveTrades() const;

    // Resource crafting
    bool canCraft(ResourceType type, float amount) const;
    bool craftResource(ResourceType type, float amount);
    std::vector<std::pair<ResourceType, float>> getCraftingRequirements(ResourceType type) const;

    // Resource storage
    bool storeResource(ResourceType type, float amount);
    bool retrieveResource(ResourceType type, float amount);
    float getStorageCapacity() const;
    float getAvailableStorage() const;

    // Transaction history
    std::vector<ResourceTransaction> getTransactionHistory() const;
    void clearTransactionHistory();

private:
    ResourceManager() = default;
    ~ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    struct ResourceState {
        float amount{0.0f};
        float gatheringRate{0.0f};
        bool isGathering{false};
        std::string gatheringSource;
    };

    std::unordered_map<ResourceType, ResourceState> resources_;
    std::vector<ResourceTransaction> transactions_;
    std::unordered_map<std::string, ResourceTransaction> activeTrades_;
    float storageCapacity_{1000.0f};
    float usedStorage_{0.0f};
};

} // namespace game
} // namespace void_contingency
```

## Key Concepts Explained

### Resource Architecture

1. **Resource Types**:

   - Basic resources
   - Crafting materials
   - Consumables
   - Currency
   - Special items

2. **Resource Properties**:

   - Value
   - Weight
   - Stackability
   - Durability
   - Rarity

3. **Resource States**:
   - Amount
   - Location
   - Condition
   - Availability

### Resource Operations

1. **Gathering**:

   - Collection rates
   - Source management
   - Capacity limits
   - Tool requirements

2. **Trading**:

   - Price calculation
   - Market dynamics
   - Trade validation
   - Transaction history

3. **Crafting**:
   - Recipe management
   - Component requirements
   - Success rates
   - Quality factors

### Resource Management

1. **Storage System**:

   - Capacity management
   - Organization
   - Access control
   - Location tracking

2. **Transaction System**:

   - Operation validation
   - History tracking
   - Rollback support
   - Error handling

3. **Economy System**:
   - Value calculation
   - Market influence
   - Supply/demand
   - Price fluctuation

## Common Pitfalls

1. **Balance Issues**:

   - Resource scarcity
   - Value inflation
   - Progression blocks
   - Economy exploits

2. **Performance Problems**:

   - Storage overhead
   - Update frequency
   - Transaction volume
   - Memory usage

3. **Design Flaws**:

   - Complex crafting
   - Unclear progression
   - Unfair trading
   - Poor feedback

4. **Technical Issues**:
   - Data consistency
   - Save corruption
   - Race conditions
   - Memory leaks

## Best Practices

1. **Resource Design**:

   - Clear purpose
   - Balanced values
   - Meaningful choices
   - Progressive complexity

2. **System Architecture**:

   - Clean interfaces
   - Efficient storage
   - Safe transactions
   - Error handling

3. **User Experience**:

   - Clear feedback
   - Easy management
   - Visual clarity
   - Helpful tooltips

4. **Performance**:
   - Optimized storage
   - Batch updates
   - Efficient queries
   - Memory management

## Testing

Create test file `tests/unit/game/resource/ResourceManagerTest.cpp`:

```cpp
TEST(ResourceManagerTest, BasicOperations) {
    auto& manager = ResourceManager::getInstance();

    // Add resources
    manager.addResource(ResourceType::Energy, 100.0f);
    EXPECT_EQ(manager.getResourceAmount(ResourceType::Energy), 100.0f);

    // Consume resources
    EXPECT_TRUE(manager.consumeResource(ResourceType::Energy, 50.0f));
    EXPECT_EQ(manager.getResourceAmount(ResourceType::Energy), 50.0f);

    // Check insufficient resources
    EXPECT_FALSE(manager.consumeResource(ResourceType::Energy, 100.0f));
    EXPECT_EQ(manager.getResourceAmount(ResourceType::Energy), 50.0f);
}

TEST(ResourceManagerTest, ResourceGathering) {
    auto& manager = ResourceManager::getInstance();

    // Start gathering
    manager.startGathering(ResourceType::Minerals, "asteroid_field");
    EXPECT_TRUE(manager.isGathering(ResourceType::Minerals));
    EXPECT_GT(manager.getGatheringRate(ResourceType::Minerals), 0.0f);

    // Stop gathering
    manager.stopGathering(ResourceType::Minerals);
    EXPECT_FALSE(manager.isGathering(ResourceType::Minerals));
    EXPECT_EQ(manager.getGatheringRate(ResourceType::Minerals), 0.0f);
}

TEST(ResourceManagerTest, ResourceTrading) {
    auto& manager = ResourceManager::getInstance();

    // Setup initial resources
    manager.addResource(ResourceType::Credits, 1000.0f);
    manager.addResource(ResourceType::Minerals, 100.0f);

    // Initiate trade
    EXPECT_TRUE(manager.initiateTrade("trader1", ResourceType::Minerals, 50.0f, 100.0f));
    auto trades = manager.getActiveTrades();
    EXPECT_EQ(trades.size(), 1);

    // Complete trade
    manager.completeTrade(trades[0].tradeId);
    EXPECT_EQ(manager.getResourceAmount(ResourceType::Minerals), 50.0f);
    EXPECT_EQ(manager.getResourceAmount(ResourceType::Credits), 900.0f);
}
```

## Next Steps

1. Add advanced features:

   - Resource quality system
   - Dynamic pricing
   - Market simulation
   - Resource conversion

2. Implement management tools:

   - Resource inspector
   - Economy monitor
   - Trading interface
   - Crafting guide

3. Create gameplay systems:

   - Resource missions
   - Trading quests
   - Crafting challenges
   - Resource events

4. Add optimization features:
   - Resource pooling
   - Transaction batching
   - Storage optimization
   - Cache management

## Additional Resources

- [Event System Tutorial](./event_system.md)
- [Combat Framework Tutorial](./combat_framework.md)
- [Economy System Guide](../guides/economy_system.md)
- [UI Framework Guide](../ui_framework/README.md)
