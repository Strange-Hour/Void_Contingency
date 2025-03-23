# Implementing the Core Game Loop

## Navigation

- Previous: [Phase 1 Documentation](../phase_one/README.md)
- Next: [Input System Tutorial](./input_system.md)

## What We're Building

We're implementing the core game loop, which is the heart of any game engine. This system manages the timing, updates, and rendering of the entire game, ensuring smooth gameplay and consistent behavior across different hardware.

## Game Development Concepts Covered

1. **Game Loop Architecture**: Understanding the fundamental structure of game engines
2. **Delta Time**: Managing frame-independent timing
3. **Fixed Time Steps**: Handling physics and other time-sensitive updates
4. **Frame Rate Management**: Controlling and monitoring game performance
5. **State Management**: Handling different game states and transitions
6. **Event Processing**: Managing game events and input
7. **Singleton Pattern**: Using global access patterns appropriately
8. **Multi-threading Considerations**: Understanding thread safety in game loops

## Why This Implementation?

1. **Reliability**: Consistent game behavior across different hardware
2. **Flexibility**: Easy to modify timing and update behavior
3. **Performance**: Efficient handling of game systems
4. **Maintainability**: Clear separation of update and render logic
5. **Debuggability**: Built-in performance monitoring

## Implementation Steps

### 1. Create Game Loop Class

Create `include/core/GameLoop.hpp`:

```cpp
#pragma once

#include "core/Time.hpp"
#include "core/EventSystem.hpp"
#include "core/StateManager.hpp"
#include <memory>
#include <chrono>

namespace void_contingency {
namespace core {

class GameLoop {
public:
    static GameLoop& getInstance();

    void initialize();
    void shutdown();
    void run();
    void stop();

    // Game state control
    void pause();
    void resume();
    void setTimeScale(float scale);
    bool isPaused() const;
    float getTimeScale() const;

    // Frame timing
    float getDeltaTime() const;
    float getFixedDeltaTime() const;
    uint64_t getFrameCount() const;
    float getFPS() const;

    // Game loop phases
    void processInput();
    void update(float deltaTime);
    void render();
    void updateFixed();

private:
    GameLoop() = default;
    ~GameLoop() = default;
    GameLoop(const GameLoop&) = delete;
    GameLoop& operator=(const GameLoop&) = delete;

    void updateTiming();
    void processEvents();
    void updateGameState();
    void renderGameState();

    bool running_{false};
    bool paused_{false};
    float timeScale_{1.0f};

    Time time_;
    EventSystem eventSystem_;
    StateManager stateManager_;

    uint64_t frameCount_{0};
    float fps_{0.0f};
    float fpsUpdateInterval_{1.0f};
    float fpsUpdateTimer_{0.0f};

    // Fixed time step
    float fixedTimeStep_{1.0f / 60.0f};  // 60 Hz
    float accumulator_{0.0f};

    // Frame timing
    std::chrono::high_resolution_clock::time_point lastFrameTime_;
    float deltaTime_{0.0f};
};

} // namespace core
} // namespace void_contingency
```

### 2. Implement Game Loop

Create `src/core/GameLoop.cpp`:

```cpp
#include "core/GameLoop.hpp"
#include "core/Logger.hpp"
#include <thread>

namespace void_contingency {
namespace core {

GameLoop& GameLoop::getInstance() {
    static GameLoop instance;
    return instance;
}

void GameLoop::initialize() {
    Logger::info("Initializing game loop...");

    // Initialize subsystems
    time_.initialize();
    eventSystem_.initialize();
    stateManager_.initialize();

    // Set initial frame time
    lastFrameTime_ = std::chrono::high_resolution_clock::now();

    Logger::info("Game loop initialized");
}

void GameLoop::shutdown() {
    Logger::info("Shutting down game loop...");

    // Shutdown subsystems
    stateManager_.shutdown();
    eventSystem_.shutdown();
    time_.shutdown();

    Logger::info("Game loop shutdown complete");
}

void GameLoop::run() {
    if (running_) {
        Logger::warning("Game loop is already running");
        return;
    }

    Logger::info("Starting game loop");
    running_ = true;

    while (running_) {
        // Update timing
        updateTiming();

        // Process input and events
        processInput();
        processEvents();

        // Update game state
        if (!paused_) {
            updateGameState();
        }

        // Render game state
        renderGameState();

        // Update frame counter
        frameCount_++;
    }
}

void GameLoop::stop() {
    if (!running_) {
        Logger::warning("Game loop is not running");
        return;
    }

    Logger::info("Stopping game loop");
    running_ = false;
}

void GameLoop::updateTiming() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto frameTime = std::chrono::duration<float>(currentTime - lastFrameTime_).count();
    lastFrameTime_ = currentTime;

    // Update delta time
    deltaTime_ = frameTime * timeScale_;

    // Update FPS counter
    fpsUpdateTimer_ += frameTime;
    if (fpsUpdateTimer_ >= fpsUpdateInterval_) {
        fps_ = frameCount_ / fpsUpdateTimer_;
        frameCount_ = 0;
        fpsUpdateTimer_ = 0.0f;
    }

    // Update fixed time step accumulator
    if (!paused_) {
        accumulator_ += deltaTime_;
    }
}

void GameLoop::processInput() {
    // Process input events
    eventSystem_.processInputEvents();
}

void GameLoop::processEvents() {
    // Process game events
    eventSystem_.processGameEvents();
}

void GameLoop::updateGameState() {
    // Update fixed time step systems
    while (accumulator_ >= fixedTimeStep_) {
        updateFixed();
        accumulator_ -= fixedTimeStep_;
    }

    // Update variable time step systems
    update(deltaTime_);
}

void GameLoop::renderGameState() {
    // Render current game state
    render();
}

void GameLoop::update(float deltaTime) {
    // Update game systems with variable time step
    stateManager_.update(deltaTime);
}

void GameLoop::updateFixed() {
    // Update physics and other fixed time step systems
    stateManager_.updateFixed(fixedTimeStep_);
}

void GameLoop::render() {
    // Render current game state
    stateManager_.render();
}

// Public interface implementations
void GameLoop::pause() {
    if (paused_) {
        Logger::warning("Game is already paused");
        return;
    }
    paused_ = true;
    Logger::info("Game paused");
}

void GameLoop::resume() {
    if (!paused_) {
        Logger::warning("Game is not paused");
        return;
    }
    paused_ = false;
    Logger::info("Game resumed");
}

void GameLoop::setTimeScale(float scale) {
    if (scale <= 0.0f) {
        Logger::warning("Invalid time scale value");
        return;
    }
    timeScale_ = scale;
    Logger::info("Time scale set to: {}", scale);
}

bool GameLoop::isPaused() const {
    return paused_;
}

float GameLoop::getTimeScale() const {
    return timeScale_;
}

float GameLoop::getDeltaTime() const {
    return deltaTime_;
}

float GameLoop::getFixedDeltaTime() const {
    return fixedTimeStep_;
}

uint64_t GameLoop::getFrameCount() const {
    return frameCount_;
}

float GameLoop::getFPS() const {
    return fps_;
}

} // namespace core
} // namespace void_contingency
```

## Key Concepts Explained

### Game Loop Architecture

1. **Main Components**:

   - Input processing
   - State updates
   - Physics updates
   - Rendering
   - Event handling

2. **Update Order**:

   - Process input first
   - Handle events
   - Update game state
   - Update physics
   - Render frame

3. **Timing System**:
   - Frame delta time
   - Fixed time step
   - Time scaling
   - FPS monitoring

### Delta Time Management

1. **Variable Time Step**:

   - Used for smooth animations
   - Handles varying frame rates
   - Scales with performance

2. **Fixed Time Step**:

   - Used for physics
   - Consistent simulation
   - Accumulator pattern

3. **Time Scaling**:
   - Slow motion effects
   - Game speed control
   - Debug features

### State Management

1. **Game States**:

   - Running
   - Paused
   - Loading
   - Menu systems

2. **State Transitions**:

   - Clean state changes
   - Resource management
   - Event handling

3. **State Updates**:
   - Independent systems
   - Priority handling
   - Performance monitoring

## Common Pitfalls

1. **Timing Issues**:

   - Frame rate dependency
   - Physics instability
   - Time step problems

2. **Performance Problems**:

   - Unoptimized updates
   - Render bottlenecks
   - Memory leaks

3. **State Management**:

   - Race conditions
   - Resource conflicts
   - Memory management

4. **Thread Safety**:
   - Input processing
   - State updates
   - Resource access

## Best Practices

1. **Timing Management**:

   - Use fixed time step for physics
   - Implement frame limiting
   - Monitor performance

2. **State Control**:

   - Clear state transitions
   - Resource cleanup
   - Error handling

3. **Performance**:

   - Profile critical paths
   - Optimize hot spots
   - Monitor memory usage

4. **Debug Support**:
   - Performance metrics
   - State monitoring
   - Error logging

## Testing

Create test file `tests/unit/core/GameLoopTest.cpp`:

```cpp
TEST(GameLoopTest, Initialization) {
    auto& gameLoop = GameLoop::getInstance();

    gameLoop.initialize();
    EXPECT_FALSE(gameLoop.isPaused());
    EXPECT_EQ(gameLoop.getTimeScale(), 1.0f);

    gameLoop.shutdown();
}

TEST(GameLoopTest, TimeManagement) {
    auto& gameLoop = GameLoop::getInstance();

    gameLoop.setTimeScale(0.5f);
    EXPECT_EQ(gameLoop.getTimeScale(), 0.5f);

    gameLoop.pause();
    EXPECT_TRUE(gameLoop.isPaused());

    gameLoop.resume();
    EXPECT_FALSE(gameLoop.isPaused());
}
```

## Next Steps

1. Add advanced features:

   - Frame interpolation
   - Multi-threading support
   - Performance profiling
   - Debug visualization

2. Implement system optimizations
3. Add monitoring tools
4. Create debugging interface

## Additional Resources

- [Input System Tutorial](./input_system.md)
- [State Management Tutorial](./state_management.md)
- [Event System Tutorial](./event_system.md)
- [Performance Optimization Guide](../guides/performance_optimization.md)
