# Implementing Combat Framework

## Navigation

- Previous: [Resource Management Tutorial](./resource_management.md)
- Next: [Upgrade System Tutorial](./upgrade_system.md)

## What We're Building

We're implementing a comprehensive combat framework that handles all aspects of combat interactions in the game. This system manages damage calculations, weapon systems, combat effects, and status tracking, providing an engaging and balanced combat experience that's both challenging and fair.

## Game Development Concepts Covered

1. **Combat Systems**: Understanding combat mechanics and flow
2. **Damage Calculation**: Implementing damage formulas and modifiers
3. **Weapon Systems**: Managing different weapon types and behaviors
4. **Combat Effects**: Handling status effects and combat modifiers
5. **Combat State**: Tracking combat status and participant states
6. **Combat Events**: Managing combat-related events and notifications
7. **Combat AI**: Implementing combat behavior and decision making
8. **Combat Balance**: Managing difficulty and progression

## Why This Implementation?

1. **Flexibility**: Support for various combat styles and weapons
2. **Balance**: Easy to adjust combat parameters and difficulty
3. **Performance**: Efficient combat calculations and updates
4. **Extensibility**: Simple to add new combat features
5. **Reliability**: Robust combat state management
6. **Feedback**: Clear combat information and effects
7. **Maintainability**: Clean separation of combat systems

## Implementation Steps

### 1. Create Combat Types

Create `include/game/combat/CombatTypes.hpp`:

```cpp
#pragma once

#include <string>
#include <vector>
#include <memory>

namespace void_contingency {
namespace game {

enum class DamageType {
    Physical,
    Energy,
    Explosive,
    Thermal,
    Electromagnetic,
    Corrosive
};

enum class CombatEffectType {
    Damage,
    Heal,
    Shield,
    Buff,
    Debuff,
    Status
};

enum class CombatStatus {
    Normal,
    Stunned,
    Disabled,
    Overcharged,
    Protected,
    Vulnerable
};

struct DamageInfo {
    DamageType type;
    float amount;
    float penetration;
    float criticalChance;
    float criticalMultiplier;
    std::string source;
};

struct CombatEffect {
    CombatEffectType type;
    float value;
    float duration;
    bool isPermanent;
    std::string source;
    std::vector<CombatStatus> appliedStatus;
};

} // namespace game
} // namespace void_contingency
```

### 2. Create Combat Manager

Create `include/game/combat/CombatManager.hpp`:

```cpp
#pragma once

#include "game/combat/CombatTypes.hpp"
#include "game/entity/Entity.hpp"
#include <vector>
#include <memory>
#include <unordered_map>

namespace void_contingency {
namespace game {

struct CombatEvent {
    enum class Type {
        Attack,
        Damage,
        Heal,
        Effect,
        Status,
        Death
    };

    Type type;
    EntityPtr source;
    EntityPtr target;
    float amount;
    std::string description;
    std::vector<CombatEffect> effects;
    std::chrono::steady_clock::time_point timestamp;
};

class CombatManager {
public:
    static CombatManager& getInstance();

    void initialize();
    void shutdown();

    // Combat state
    void startCombat(const std::vector<EntityPtr>& participants);
    void endCombat();
    bool isInCombat() const;
    const std::vector<EntityPtr>& getCombatParticipants() const;
    float getCombatDuration() const;

    // Combat actions
    bool performAttack(EntityPtr attacker, EntityPtr target, const std::string& weaponId);
    bool applyDamage(EntityPtr source, EntityPtr target, const DamageInfo& damage);
    bool applyHealing(EntityPtr source, EntityPtr target, float amount);
    bool applyEffect(EntityPtr source, EntityPtr target, const CombatEffect& effect);
    bool applyStatus(EntityPtr target, CombatStatus status, float duration);

    // Combat queries
    bool canAttack(EntityPtr attacker, EntityPtr target) const;
    bool isValidTarget(EntityPtr entity) const;
    float getEffectiveDamage(const DamageInfo& damage, EntityPtr target) const;
    std::vector<CombatEffect> getActiveEffects(EntityPtr entity) const;
    std::vector<CombatStatus> getActiveStatus(EntityPtr entity) const;

    // Combat statistics
    float getTotalDamageDealt(EntityPtr entity) const;
    float getTotalDamageReceived(EntityPtr entity) const;
    float getTotalHealingDone(EntityPtr entity) const;
    int getKillCount(EntityPtr entity) const;
    float getAverageDPS(EntityPtr entity) const;

    // Combat events
    void registerCombatEventHandler(std::function<void(const CombatEvent&)> handler);
    void unregisterCombatEventHandler(std::function<void(const CombatEvent&)> handler);
    std::vector<CombatEvent> getCombatHistory() const;

private:
    CombatManager() = default;
    ~CombatManager() = default;
    CombatManager(const CombatManager&) = delete;
    CombatManager& operator=(const CombatManager&) = delete;

    struct CombatState {
        bool active{false};
        float duration{0.0f};
        std::vector<EntityPtr> participants;
        std::unordered_map<EntityPtr, std::vector<CombatEffect>> activeEffects;
        std::unordered_map<EntityPtr, std::vector<CombatStatus>> activeStatus;
        std::vector<CombatEvent> history;
    };

    struct CombatStats {
        float damageDealt{0.0f};
        float damageReceived{0.0f};
        float healingDone{0.0f};
        int kills{0};
        float combatTime{0.0f};
    };

    void updateCombatState(float deltaTime);
    void processCombatEffects(float deltaTime);
    void updateCombatStats();
    void notifyCombatEvent(const CombatEvent& event);
    float calculateDamage(const DamageInfo& damage, EntityPtr target) const;
    bool validateCombatAction(EntityPtr source, EntityPtr target) const;

    CombatState state_;
    std::unordered_map<EntityPtr, CombatStats> stats_;
    std::vector<std::function<void(const CombatEvent&)>> eventHandlers_;
};

} // namespace game
} // namespace void_contingency
```

## Key Concepts Explained

### Combat Architecture

1. **Damage System**:

   - Damage types
   - Damage calculation
   - Damage modifiers
   - Critical hits
   - Penetration

2. **Effect System**:

   - Status effects
   - Buffs/Debuffs
   - Duration tracking
   - Effect stacking
   - Effect removal

3. **Combat Flow**:
   - Initiative
   - Turn order
   - Action points
   - Reaction system
   - Combat phases

### Combat Mechanics

1. **Attack System**:

   - Hit calculation
   - Dodge/Block
   - Counter attacks
   - Combo system
   - Special moves

2. **Defense System**:

   - Armor types
   - Shield system
   - Damage reduction
   - Resistance
   - Recovery

3. **Status System**:
   - Status types
   - Status duration
   - Status immunity
   - Status removal
   - Status effects

### Combat Management

1. **State Tracking**:

   - Combat phases
   - Participant status
   - Effect duration
   - Combat history
   - Statistics

2. **Event System**:

   - Combat events
   - Event handlers
   - Event history
   - Event validation
   - Event effects

3. **Balance System**:
   - Damage scaling
   - Difficulty adjustment
   - Power curves
   - Progression
   - Rewards

## Common Pitfalls

1. **Balance Issues**:

   - Overpowered combinations
   - Damage scaling
   - Difficulty spikes
   - Unfair mechanics
   - Exploit potential

2. **Performance Problems**:

   - Effect processing
   - Event handling
   - State updates
   - Memory usage
   - Network sync

3. **Design Flaws**:

   - Complex mechanics
   - Unclear feedback
   - Poor pacing
   - Frustrating systems
   - Lack of strategy

4. **Technical Issues**:
   - State corruption
   - Event ordering
   - Race conditions
   - Memory leaks
   - Network latency

## Best Practices

1. **Combat Design**:

   - Clear mechanics
   - Balanced systems
   - Strategic depth
   - Fair challenge
   - Good feedback

2. **System Architecture**:

   - Clean interfaces
   - Efficient processing
   - Safe state management
   - Error handling
   - Event validation

3. **User Experience**:

   - Clear feedback
   - Visual effects
   - Sound effects
   - UI clarity
   - Control responsiveness

4. **Performance**:
   - Optimized calculations
   - Efficient state tracking
   - Event batching
   - Memory management
   - Network optimization

## Testing

Create test file `tests/unit/game/combat/CombatManagerTest.cpp`:

```cpp
TEST(CombatManagerTest, BasicCombat) {
    auto& manager = CombatManager::getInstance();

    // Create entities
    auto attacker = std::make_shared<Entity>("Attacker");
    auto target = std::make_shared<Entity>("Target");

    // Start combat
    manager.startCombat({attacker, target});
    EXPECT_TRUE(manager.isInCombat());

    // Perform attack
    DamageInfo damage{
        DamageType::Physical,
        100.0f,
        0.2f,
        0.1f,
        2.0f,
        "Test Weapon"
    };

    EXPECT_TRUE(manager.applyDamage(attacker, target, damage));
    EXPECT_GT(manager.getTotalDamageDealt(attacker), 0.0f);
    EXPECT_GT(manager.getTotalDamageReceived(target), 0.0f);
}

TEST(CombatManagerTest, CombatEffects) {
    auto& manager = CombatManager::getInstance();

    // Create entities
    auto source = std::make_shared<Entity>("Source");
    auto target = std::make_shared<Entity>("Target");

    // Apply effect
    CombatEffect effect{
        CombatEffectType::Debuff,
        0.5f,
        5.0f,
        false,
        "Test Effect",
        {CombatStatus::Vulnerable}
    };

    EXPECT_TRUE(manager.applyEffect(source, target, effect));
    auto activeEffects = manager.getActiveEffects(target);
    EXPECT_EQ(activeEffects.size(), 1);

    auto activeStatus = manager.getActiveStatus(target);
    EXPECT_EQ(activeStatus.size(), 1);
    EXPECT_EQ(activeStatus[0], CombatStatus::Vulnerable);
}

TEST(CombatManagerTest, CombatEvents) {
    auto& manager = CombatManager::getInstance();

    bool eventReceived = false;
    manager.registerCombatEventHandler([&eventReceived](const CombatEvent& event) {
        eventReceived = true;
        EXPECT_EQ(event.type, CombatEvent::Type::Damage);
    });

    // Create and apply damage
    auto source = std::make_shared<Entity>("Source");
    auto target = std::make_shared<Entity>("Target");

    DamageInfo damage{DamageType::Energy, 50.0f, 0.0f, 0.0f, 1.0f, "Test"};
    manager.applyDamage(source, target, damage);

    EXPECT_TRUE(eventReceived);
}
```

## Next Steps

1. Add advanced features:

   - Combo system
   - Counter system
   - Special abilities
   - Ultimate moves

2. Implement combat tools:

   - Combat visualizer
   - Damage analyzer
   - Balance tester
   - Combat replay

3. Create combat systems:

   - AI behaviors
   - Team tactics
   - Formation system
   - Cover system

4. Add optimization features:
   - Effect pooling
   - Event batching
   - State caching
   - Network prediction

## Additional Resources

- [Resource Management Tutorial](./resource_management.md)
- [Upgrade System Tutorial](./upgrade_system.md)
