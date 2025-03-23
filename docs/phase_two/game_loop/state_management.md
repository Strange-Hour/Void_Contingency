# Implementing State Management

## Navigation

- Previous: [Event System Tutorial](./event_system.md)
- Next: [Input System Tutorial](./input_system.md)

## What We're Building

We're implementing a comprehensive state management system that handles game states, transitions, and persistence. This system manages different game states (like menus, gameplay, loading screens), their transitions, and their data, providing a robust foundation for managing game flow and user interactions.

## Game Development Concepts Covered

1. **State Architecture**:

   - State types
   - State hierarchy
   - State transitions
   - State persistence
   - State validation

2. **State Processing**:

   - State updates
   - State rendering
   - State input
   - State events
   - State timing

3. **State Management**:
   - State registration
   - State switching
   - State stacking
   - State history
   - State debugging

## Why This Implementation?

1. **Flexibility**: Support for various state types and transitions
2. **Performance**: Efficient state processing and switching
3. **Extensibility**: Easy to add new states
4. **Reliability**: Robust state handling and recovery
5. **Maintainability**: Clean state architecture
6. **Debugability**: Clear state tracking and logging
7. **Scalability**: Support for complex state hierarchies

## Implementation Steps

### 1. Create State Types

Create `include/game/state/StateTypes.hpp`:

```cpp
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <functional>
#include <unordered_map>

namespace void_contingency {
namespace game {

enum class StateType {
    Menu,
    Game,
    Loading,
    Pause,
    Dialog,
    Settings,
    Credits
};

enum class TransitionType {
    None,
    Fade,
    Slide,
    Zoom,
    Custom
};

struct StateData {
    std::string id;
    std::string name;
    StateType type;
    std::unordered_map<std::string, std::string> parameters;
    std::chrono::steady_clock::time_point timestamp;
};

struct TransitionData {
    TransitionType type;
    float duration;
    std::unordered_map<std::string, float> parameters;
};

class State {
public:
    State(StateType type)
        : type_(type), active_(false), paused_(false) {}

    virtual ~State() = default;

    // State lifecycle
    virtual void initialize() = 0;
    virtual void cleanup() = 0;
    virtual void pause() { paused_ = true; }
    virtual void resume() { paused_ = false; }

    // State processing
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void handleInput() = 0;
    virtual void handleEvent(const std::string& event) = 0;

    // State properties
    StateType getType() const { return type_; }
    bool isActive() const { return active_; }
    bool isPaused() const { return paused_; }
    const StateData& getData() const { return data_; }
    void setData(const StateData& data) { data_ = data; }

protected:
    StateType type_;
    bool active_;
    bool paused_;
    StateData data_;
};

using StatePtr = std::shared_ptr<State>;
using StateFactory = std::function<StatePtr()>;

} // namespace game
} // namespace void_contingency
```

### 2. Create State Manager

Create `include/game/state/StateManager.hpp`:

```cpp
#pragma once

#include "game/state/StateTypes.hpp"
#include <vector>
#include <stack>
#include <memory>
#include <mutex>

namespace void_contingency {
namespace game {

class StateManager {
public:
    static StateManager& getInstance();

    void initialize();
    void shutdown();

    // State registration
    void registerState(StateType type, StateFactory factory);
    void unregisterState(StateType type);
    bool isStateRegistered(StateType type) const;

    // State operations
    void pushState(StateType type, const StateData& data = StateData());
    void popState();
    void changeState(StateType type, const StateData& data = StateData());
    void clearStates();

    // State access
    StatePtr getCurrentState() const;
    StatePtr getPreviousState() const;
    std::vector<StatePtr> getStateStack() const;
    int getStateCount() const;

    // State queries
    bool hasState() const;
    bool isStateActive(StateType type) const;
    StateType getCurrentStateType() const;

    // State processing
    void update(float deltaTime);
    void render();
    void handleInput();
    void handleEvent(const std::string& event);

    // State transitions
    void setTransition(const TransitionData& transition);
    bool isTransitioning() const;
    float getTransitionProgress() const;

    // State persistence
    void saveStateStack(const std::string& filename);
    void loadStateStack(const std::string& filename);
    void clearStateHistory();

private:
    StateManager() = default;
    ~StateManager() = default;
    StateManager(const StateManager&) = delete;
    StateManager& operator=(const StateManager&) = delete;

    struct TransitionState {
        TransitionData data;
        float progress;
        StatePtr from;
        StatePtr to;
    };

    void processStateChange(StatePtr newState, const StateData& data);
    void updateTransition(float deltaTime);
    void cleanupState(StatePtr state);
    void notifyStateChange(StatePtr oldState, StatePtr newState);
    bool validateStateTransition(StateType from, StateType to) const;

    std::unordered_map<StateType, StateFactory> factories_;
    std::stack<StatePtr> states_;
    std::vector<StatePtr> history_;
    TransitionState transition_;
    std::mutex mutex_;
};

} // namespace game
} // namespace void_contingency
```

## Key Concepts Explained

### State Architecture

1. **State Types**:

   - Menu states
   - Game states
   - Loading states
   - Dialog states
   - Settings states

2. **State Lifecycle**:

   - Initialization
   - Cleanup
   - Pause/Resume
   - Update/Render

3. **State Data**:
   - State ID
   - Parameters
   - Timestamp
   - Properties

### State Processing

1. **Update System**:

   - Delta time
   - Input handling
   - Event processing
   - State logic
   - Transitions

2. **Render System**:

   - State rendering
   - Transition effects
   - UI elements
   - Debug info
   - Performance

3. **Input System**:
   - Input handling
   - Event routing
   - Focus management
   - Input blocking
   - Input states

### State Management

1. **Stack System**:

   - State pushing
   - State popping
   - State changing
   - Stack clearing
   - History tracking

2. **Transition System**:

   - Transition types
   - Transition timing
   - Effect parameters
   - State blending
   - Cancellation

3. **Persistence System**:
   - State saving
   - State loading
   - History tracking
   - Data validation
   - Error recovery

## Common Pitfalls

1. **State Issues**:

   - Memory leaks
   - Invalid transitions
   - Stack corruption
   - Resource leaks
   - State conflicts

2. **Performance Issues**:

   - Slow transitions
   - Memory bloat
   - Update bottlenecks
   - Render issues
   - Input lag

3. **Design Issues**:

   - Complex states
   - Deep stacks
   - Unclear flow
   - Poor feedback
   - State confusion

4. **Technical Issues**:
   - Thread safety
   - Race conditions
   - Memory management
   - Resource handling
   - Error recovery

## Best Practices

1. **State Design**:

   - Clear purpose
   - Simple transitions
   - Good feedback
   - Clean cleanup
   - Error handling

2. **Performance**:

   - Efficient updates
   - Fast transitions
   - Resource management
   - Memory optimization
   - State pooling

3. **Architecture**:

   - Clean hierarchy
   - Clear transitions
   - Good separation
   - Easy debugging
   - Safe persistence

4. **User Experience**:
   - Smooth transitions
   - Clear feedback
   - Responsive input
   - Good performance
   - Error recovery

## Testing

Create test file `tests/unit/game/state/StateManagerTest.cpp`:

```cpp
TEST(StateManagerTest, BasicStateOperations) {
    auto& manager = StateManager::getInstance();

    // Register state factory
    bool stateInitialized = false;
    manager.registerState(StateType::Menu, [&stateInitialized]() {
        auto state = std::make_shared<MenuState>();
        stateInitialized = true;
        return state;
    });

    // Push state
    manager.pushState(StateType::Menu);
    EXPECT_TRUE(stateInitialized);
    EXPECT_EQ(manager.getCurrentStateType(), StateType::Menu);

    // Pop state
    manager.popState();
    EXPECT_FALSE(manager.hasState());
}

TEST(StateManagerTest, StateStack) {
    auto& manager = StateManager::getInstance();

    // Push multiple states
    manager.pushState(StateType::Menu);
    manager.pushState(StateType::Game);
    manager.pushState(StateType::Pause);

    // Check stack
    EXPECT_EQ(manager.getStateCount(), 3);
    EXPECT_EQ(manager.getCurrentStateType(), StateType::Pause);

    // Pop state
    manager.popState();
    EXPECT_EQ(manager.getCurrentStateType(), StateType::Game);

    // Clear states
    manager.clearStates();
    EXPECT_FALSE(manager.hasState());
}

TEST(StateManagerTest, StateTransitions) {
    auto& manager = StateManager::getInstance();

    // Set up transition
    TransitionData transition{
        TransitionType::Fade,
        0.5f,
        {{"alpha", 1.0f}}
    };
    manager.setTransition(transition);

    // Change state with transition
    manager.pushState(StateType::Menu);
    EXPECT_TRUE(manager.isTransitioning());

    // Update transition
    manager.update(0.25f);
    EXPECT_EQ(manager.getTransitionProgress(), 0.5f);

    manager.update(0.25f);
    EXPECT_FALSE(manager.isTransitioning());
}
```

## Next Steps

1. Add advanced features:

   - State prediction
   - State compression
   - State validation
   - State replay

2. Implement state tools:

   - State viewer
   - State debugger
   - State profiler
   - State analyzer

3. Create state systems:

   - Complex transitions
   - State hierarchies
   - State persistence
   - State networking

4. Add optimization features:
   - State pooling
   - Transition pooling
   - Memory optimization
   - Update optimization

## Additional Resources

- [Event System Tutorial](./event_system.md)
- [Input System Tutorial](./input_system.md)
- [Event System Tutorial](./event_system.md)
