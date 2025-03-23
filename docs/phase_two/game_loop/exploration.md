# Implementing Exploration System

## Navigation

- Previous: [Sector Generation Tutorial](./sector_generation.md)
- Next: [Resource Management Tutorial](./resource_management.md)

## What We're Building

We're implementing a comprehensive exploration system that manages how players discover and interact with the game world. This system handles map generation, discovery mechanics, points of interest, and exploration rewards, creating an engaging and dynamic exploration experience.

## Game Development Concepts Covered

1. **World Generation**:

   - Procedural generation
   - Map layouts
   - Point of interest placement
   - Resource distribution
   - Difficulty scaling

2. **Discovery Mechanics**:

   - Fog of war
   - Scanning systems
   - Map markers
   - Exploration progress
   - Area visibility

3. **Interaction Systems**:
   - Point of interest interaction
   - Resource collection
   - Event triggers
   - Quest integration
   - Reward systems

## Why This Implementation?

1. **Flexibility**: Support for various exploration mechanics
2. **Replayability**: Dynamic content generation
3. **Performance**: Efficient world management
4. **Extensibility**: Easy to add new features
5. **Balance**: Adjustable exploration rewards
6. **Feedback**: Clear discovery indicators
7. **Maintainability**: Clean system separation

## Implementation Steps

### 1. Create Exploration Types

Create `include/game/exploration/ExplorationTypes.hpp`:

```cpp
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace void_contingency {
namespace game {

enum class ExplorationAreaType {
    Unknown,
    Safe,
    Dangerous,
    Resource,
    Quest,
    Special
};

enum class PointOfInterestType {
    Station,
    Asteroid,
    Wreckage,
    Anomaly,
    Cache,
    Base
};

enum class DiscoveryType {
    Area,
    Resource,
    Enemy,
    Quest,
    Secret
};

struct ExplorationReward {
    std::string id;
    std::string type;
    float amount;
    bool isUnique;
    std::string description;
};

struct PointOfInterest {
    std::string id;
    std::string name;
    PointOfInterestType type;
    Vector2D position;
    bool isDiscovered;
    bool isVisited;
    std::vector<ExplorationReward> rewards;
    std::unordered_map<std::string, float> properties;
};

struct ExplorationArea {
    std::string id;
    std::string name;
    ExplorationAreaType type;
    Rectangle bounds;
    float dangerLevel;
    bool isExplored;
    std::vector<PointOfInterest> pointsOfInterest;
    std::unordered_map<std::string, float> resources;
};

} // namespace game
} // namespace void_contingency
```

### 2. Create Exploration Manager

Create `include/game/exploration/ExplorationManager.hpp`:

```cpp
#pragma once

#include "game/exploration/ExplorationTypes.hpp"
#include "game/entity/Entity.hpp"
#include <vector>
#include <memory>
#include <unordered_map>

namespace void_contingency {
namespace game {

class ExplorationManager {
public:
    static ExplorationManager& getInstance();

    void initialize();
    void shutdown();

    // Area management
    void registerArea(const ExplorationArea& area);
    void unregisterArea(const std::string& areaId);
    bool hasArea(const std::string& areaId) const;
    ExplorationArea* getArea(const std::string& areaId);
    std::vector<ExplorationArea*> getAreasInRange(const Vector2D& position, float range);

    // Point of interest management
    void registerPointOfInterest(const std::string& areaId, const PointOfInterest& poi);
    void unregisterPointOfInterest(const std::string& areaId, const std::string& poiId);
    PointOfInterest* getPointOfInterest(const std::string& areaId, const std::string& poiId);
    std::vector<PointOfInterest*> getPointsOfInterestInRange(const Vector2D& position, float range);

    // Exploration mechanics
    void updateExploration(EntityPtr entity, float deltaTime);
    void discoverArea(EntityPtr entity, const std::string& areaId);
    void visitPointOfInterest(EntityPtr entity, const std::string& areaId, const std::string& poiId);
    void collectResource(EntityPtr entity, const std::string& areaId, const std::string& resourceId);

    // Discovery queries
    bool isAreaDiscovered(EntityPtr entity, const std::string& areaId) const;
    bool isPointOfInterestDiscovered(EntityPtr entity, const std::string& areaId, const std::string& poiId) const;
    float getExplorationProgress(EntityPtr entity, const std::string& areaId) const;
    std::vector<DiscoveryType> getRecentDiscoveries(EntityPtr entity) const;

    // Resource queries
    std::unordered_map<std::string, float> getAvailableResources(const std::string& areaId) const;
    bool canCollectResource(EntityPtr entity, const std::string& areaId, const std::string& resourceId) const;
    float getResourceAmount(const std::string& areaId, const std::string& resourceId) const;

    // Reward system
    void registerReward(const std::string& areaId, const std::string& poiId, const ExplorationReward& reward);
    void claimReward(EntityPtr entity, const std::string& areaId, const std::string& poiId, const std::string& rewardId);
    std::vector<ExplorationReward> getAvailableRewards(EntityPtr entity, const std::string& areaId, const std::string& poiId) const;

    // Statistics
    float getTotalExplorationProgress(EntityPtr entity) const;
    int getDiscoveredAreaCount(EntityPtr entity) const;
    int getVisitedPointOfInterestCount(EntityPtr entity) const;
    std::unordered_map<std::string, float> getCollectedResources(EntityPtr entity) const;

    // Persistence
    void saveExplorationState(EntityPtr entity, const std::string& filename);
    void loadExplorationState(EntityPtr entity, const std::string& filename);

private:
    ExplorationManager() = default;
    ~ExplorationManager() = default;
    ExplorationManager(const ExplorationManager&) = delete;
    ExplorationManager& operator=(const ExplorationManager&) = delete;

    struct ExplorationState {
        std::unordered_map<std::string, bool> discoveredAreas;
        std::unordered_map<std::string, std::unordered_map<std::string, bool>> discoveredPOIs;
        std::unordered_map<std::string, float> areaProgress;
        std::vector<DiscoveryType> recentDiscoveries;
        std::unordered_map<std::string, float> collectedResources;
        std::unordered_map<std::string, std::vector<std::string>> claimedRewards;
    };

    void updateVisibility(EntityPtr entity, const Vector2D& position);
    void processDiscovery(EntityPtr entity, DiscoveryType type, const std::string& areaId);
    void updateAreaProgress(EntityPtr entity, const std::string& areaId);
    bool validateResourceCollection(EntityPtr entity, const std::string& areaId, const std::string& resourceId);
    void notifyDiscovery(EntityPtr entity, DiscoveryType type, const std::string& description);

    std::unordered_map<std::string, ExplorationArea> areas_;
    std::unordered_map<EntityPtr, ExplorationState> entityStates_;
};

} // namespace game
} // namespace void_contingency
```

## Key Concepts Explained

### Exploration Architecture

1. **Area System**:

   - Area types
   - Area boundaries
   - Area discovery
   - Area progress
   - Area resources

2. **Point of Interest System**:

   - POI types
   - POI placement
   - POI discovery
   - POI interaction
   - POI rewards

3. **Discovery System**:
   - Discovery types
   - Discovery mechanics
   - Discovery tracking
   - Discovery rewards
   - Discovery events

### Exploration Mechanics

1. **Visibility System**:

   - Fog of war
   - Line of sight
   - Scan range
   - Visibility updates
   - Map markers

2. **Resource System**:

   - Resource types
   - Resource placement
   - Resource collection
   - Resource limits
   - Resource respawn

3. **Reward System**:
   - Reward types
   - Reward distribution
   - Reward claiming
   - Unique rewards
   - Reward scaling

### Exploration Management

1. **State Tracking**:

   - Area states
   - POI states
   - Discovery history
   - Resource tracking
   - Progress tracking

2. **Progress System**:

   - Area completion
   - POI completion
   - Resource collection
   - Achievement tracking
   - Statistics

3. **Event System**:
   - Discovery events
   - Interaction events
   - Resource events
   - Reward events
   - Progress events

## Common Pitfalls

1. **Generation Issues**:

   - Poor distribution
   - Unreachable areas
   - Resource imbalance
   - Repetitive content
   - Performance problems

2. **Gameplay Issues**:

   - Unclear progression
   - Boring exploration
   - Unfair rewards
   - Tedious mechanics
   - Poor feedback

3. **Technical Issues**:

   - State corruption
   - Memory leaks
   - Update bottlenecks
   - Save/load errors
   - Sync problems

4. **Design Issues**:
   - Unclear objectives
   - Poor pacing
   - Limited variety
   - Unfair mechanics
   - Lack of rewards

## Best Practices

1. **World Design**:

   - Interesting layouts
   - Balanced distribution
   - Clear objectives
   - Good pacing
   - Meaningful rewards

2. **Technical Implementation**:

   - Efficient updates
   - Clean architecture
   - Safe persistence
   - Error handling
   - Performance optimization

3. **User Experience**:

   - Clear feedback
   - Good visibility
   - Easy navigation
   - Helpful markers
   - Progress tracking

4. **Content Design**:
   - Varied content
   - Balanced rewards
   - Interesting POIs
   - Dynamic events
   - Meaningful choices

## Testing

Create test file `tests/unit/game/exploration/ExplorationManagerTest.cpp`:

```cpp
TEST(ExplorationManagerTest, BasicExploration) {
    auto& manager = ExplorationManager::getInstance();

    // Create entity and area
    auto entity = std::make_shared<Entity>("Explorer");
    ExplorationArea area{
        "test_area",
        "Test Area",
        ExplorationAreaType::Safe,
        Rectangle{0, 0, 100, 100},
        0.0f,
        false,
        {},
        {{"minerals", 100.0f}}
    };

    // Register area
    manager.registerArea(area);
    EXPECT_TRUE(manager.hasArea("test_area"));

    // Discover area
    manager.discoverArea(entity, "test_area");
    EXPECT_TRUE(manager.isAreaDiscovered(entity, "test_area"));
    EXPECT_GT(manager.getExplorationProgress(entity, "test_area"), 0.0f);
}

TEST(ExplorationManagerTest, PointOfInterest) {
    auto& manager = ExplorationManager::getInstance();
    auto entity = std::make_shared<Entity>("Explorer");

    // Create and register area
    ExplorationArea area{
        "test_area",
        "Test Area",
        ExplorationAreaType::Safe,
        Rectangle{0, 0, 100, 100},
        0.0f,
        false,
        {},
        {}
    };
    manager.registerArea(area);

    // Create and register POI
    PointOfInterest poi{
        "test_poi",
        "Test POI",
        PointOfInterestType::Station,
        Vector2D{50, 50},
        false,
        false,
        {{
            "test_reward",
            "credits",
            100.0f,
            true,
            "Test reward"
        }},
        {}
    };
    manager.registerPointOfInterest("test_area", poi);

    // Visit POI
    manager.visitPointOfInterest(entity, "test_area", "test_poi");
    EXPECT_TRUE(manager.isPointOfInterestDiscovered(entity, "test_area", "test_poi"));

    // Check rewards
    auto rewards = manager.getAvailableRewards(entity, "test_area", "test_poi");
    EXPECT_EQ(rewards.size(), 1);
    EXPECT_EQ(rewards[0].amount, 100.0f);
}

TEST(ExplorationManagerTest, ResourceCollection) {
    auto& manager = ExplorationManager::getInstance();
    auto entity = std::make_shared<Entity>("Explorer");

    // Create and register area with resources
    ExplorationArea area{
        "test_area",
        "Test Area",
        ExplorationAreaType::Resource,
        Rectangle{0, 0, 100, 100},
        0.0f,
        false,
        {},
        {{"minerals", 100.0f}}
    };
    manager.registerArea(area);

    // Check resource availability
    auto resources = manager.getAvailableResources("test_area");
    EXPECT_EQ(resources["minerals"], 100.0f);

    // Collect resources
    EXPECT_TRUE(manager.canCollectResource(entity, "test_area", "minerals"));
    manager.collectResource(entity, "test_area", "minerals");

    // Check collected resources
    auto collected = manager.getCollectedResources(entity);
    EXPECT_GT(collected["minerals"], 0.0f);
}
```

## Next Steps

1. Add advanced features:

   - Dynamic events
   - Weather system
   - Day/night cycle
   - Faction territories

2. Implement exploration tools:

   - Map editor
   - POI editor
   - Resource editor
   - Event editor

3. Create exploration systems:

   - Faction influence
   - Territory control
   - Dynamic events
   - Weather effects

4. Add optimization features:
   - Area streaming
   - LOD system
   - Visibility culling
   - State compression

## Additional Resources

- [Upgrade System Tutorial](./upgrade_system.md)
- [Sector Generation Tutorial](./sector_generation.md)
