# Implementing Event System

## Navigation

- Previous: [Sector Generation Tutorial](./sector_generation.md)
- Next: [State Management Tutorial](./state_management.md)

## What We're Building

We're implementing a comprehensive event system that manages game events, triggers, and responses. This system handles event creation, distribution, processing, and response management, providing a flexible and powerful way to handle game state changes and interactions.

## Game Development Concepts Covered

1. **Event Architecture**:

   - Event types
   - Event queues
   - Event handlers
   - Event priorities
   - Event filtering

2. **Event Processing**:

   - Event dispatch
   - Event propagation
   - Event cancellation
   - Event batching
   - Event timing

3. **Event Management**:
   - Event registration
   - Event subscription
   - Event validation
   - Event logging
   - Event debugging

## Why This Implementation?

1. **Flexibility**: Support for various event types and handlers
2. **Performance**: Efficient event processing and distribution
3. **Extensibility**: Easy to add new event types
4. **Reliability**: Robust event handling and error recovery
5. **Maintainability**: Clean event architecture
6. **Debugability**: Clear event tracking and logging
7. **Scalability**: Support for high event volumes

## Implementation Steps

### 1. Create Event Types

Create `include/game/event/EventTypes.hpp`:

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

enum class EventType {
    Game,
    Input,
    Entity,
    Combat,
    Resource,
    UI,
    Network,
    System
};

enum class EventPriority {
    Low,
    Normal,
    High,
    Critical
};

struct EventData {
    std::string id;
    std::string source;
    std::string target;
    std::unordered_map<std::string, std::string> parameters;
    std::chrono::steady_clock::time_point timestamp;
};

struct EventFilter {
    std::vector<EventType> types;
    std::vector<std::string> sources;
    std::vector<std::string> targets;
    EventPriority minPriority;
    bool allowCancelled;
};

class Event {
public:
    Event(EventType type, EventPriority priority = EventPriority::Normal)
        : type_(type), priority_(priority), cancelled_(false) {}

    virtual ~Event() = default;

    EventType getType() const { return type_; }
    EventPriority getPriority() const { return priority_; }
    bool isCancelled() const { return cancelled_; }
    void setCancelled(bool cancelled) { cancelled_ = cancelled; }

    const EventData& getData() const { return data_; }
    void setData(const EventData& data) { data_ = data; }

protected:
    EventType type_;
    EventPriority priority_;
    bool cancelled_;
    EventData data_;
};

using EventPtr = std::shared_ptr<Event>;
using EventHandler = std::function<void(EventPtr)>;

} // namespace game
} // namespace void_contingency
```

### 2. Create Event Manager

Create `include/game/event/EventManager.hpp`:

```cpp
#pragma once

#include "game/event/EventTypes.hpp"
#include <vector>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>

namespace void_contingency {
namespace game {

class EventManager {
public:
    static EventManager& getInstance();

    void initialize();
    void shutdown();

    // Event registration
    void registerEventType(EventType type, const std::string& name);
    void unregisterEventType(EventType type);
    bool isEventTypeRegistered(EventType type) const;

    // Event handling
    void addEventHandler(EventType type, EventHandler handler, EventPriority priority = EventPriority::Normal);
    void removeEventHandler(EventType type, EventHandler handler);
    void clearEventHandlers(EventType type);

    // Event processing
    void queueEvent(EventPtr event);
    void processEvents();
    void processEventsAsync();
    void waitForEvents();
    void cancelEvent(const std::string& eventId);
    void cancelAllEvents();

    // Event filtering
    void addEventFilter(const EventFilter& filter);
    void removeEventFilter(const EventFilter& filter);
    bool passesFilter(EventPtr event, const EventFilter& filter) const;

    // Event queries
    std::vector<EventPtr> getQueuedEvents() const;
    std::vector<EventPtr> getProcessedEvents() const;
    int getQueueSize() const;
    bool hasEvents() const;

    // Event monitoring
    void enableEventLogging(bool enable);
    void setLogLevel(EventPriority minPriority);
    void dumpEventLog(const std::string& filename);
    void clearEventLog();

private:
    EventManager() = default;
    ~EventManager() = default;
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;

    struct EventQueueItem {
        EventPtr event;
        std::chrono::steady_clock::time_point timestamp;

        bool operator<(const EventQueueItem& other) const {
            if (event->getPriority() != other.event->getPriority()) {
                return event->getPriority() < other.event->getPriority();
            }
            return timestamp > other.timestamp;
        }
    };

    void processEvent(EventPtr event);
    void logEvent(EventPtr event, const std::string& status);
    bool validateEvent(EventPtr event) const;
    void notifyEventProcessed(EventPtr event);

    std::unordered_map<EventType, std::string> eventTypes_;
    std::unordered_map<EventType, std::vector<std::pair<EventHandler, EventPriority>>> handlers_;
    std::priority_queue<EventQueueItem> eventQueue_;
    std::vector<EventPtr> processedEvents_;
    std::vector<EventFilter> filters_;
    std::vector<std::pair<EventPtr, std::string>> eventLog_;

    bool logging_{false};
    EventPriority logLevel_{EventPriority::Normal};
    std::mutex mutex_;
    std::condition_variable condition_;
    bool processing_{false};
};

} // namespace game
} // namespace void_contingency
```

## Key Concepts Explained

### Event Architecture

1. **Event Types**:

   - Game events
   - Input events
   - Entity events
   - Combat events
   - Resource events

2. **Event Priorities**:

   - Critical events
   - High priority
   - Normal priority
   - Low priority

3. **Event Data**:
   - Event ID
   - Source/Target
   - Parameters
   - Timestamp

### Event Processing

1. **Queue System**:

   - Priority queue
   - Event ordering
   - Queue management
   - Batch processing
   - Queue monitoring

2. **Handler System**:

   - Handler registration
   - Handler execution
   - Handler priorities
   - Handler removal
   - Error handling

3. **Filter System**:
   - Type filtering
   - Source filtering
   - Target filtering
   - Priority filtering
   - Status filtering

### Event Management

1. **State Control**:

   - Queue state
   - Processing state
   - Handler state
   - Filter state
   - Log state

2. **Threading**:

   - Async processing
   - Thread safety
   - Synchronization
   - Event waiting
   - Cancellation

3. **Monitoring**:
   - Event logging
   - Performance tracking
   - Error detection
   - State validation
   - Debug support

## Common Pitfalls

1. **Performance Issues**:

   - Queue overflow
   - Handler bottlenecks
   - Memory leaks
   - Thread contention
   - Processing delays

2. **Design Issues**:

   - Complex handlers
   - Tight coupling
   - Circular events
   - Missing handlers
   - Event storms

3. **Threading Issues**:

   - Race conditions
   - Deadlocks
   - Starvation
   - Priority inversion
   - Thread safety

4. **Maintenance Issues**:
   - Handler sprawl
   - Event pollution
   - Poor logging
   - Debug difficulty
   - State corruption

## Best Practices

1. **Event Design**:

   - Clear types
   - Simple handlers
   - Good priorities
   - Clean data
   - Error handling

2. **Performance**:

   - Efficient queuing
   - Fast processing
   - Memory management
   - Thread pooling
   - Event batching

3. **Reliability**:

   - Error recovery
   - State validation
   - Safe threading
   - Good logging
   - Clear debugging

4. **Maintainability**:
   - Clean architecture
   - Good documentation
   - Clear naming
   - Easy debugging
   - Simple testing

## Testing

Create test file `tests/unit/game/event/EventManagerTest.cpp`:

```cpp
TEST(EventManagerTest, BasicEventHandling) {
    auto& manager = EventManager::getInstance();

    // Register event type
    manager.registerEventType(EventType::Game, "GameEvent");
    EXPECT_TRUE(manager.isEventTypeRegistered(EventType::Game));

    // Create and queue event
    bool eventHandled = false;
    auto handler = [&eventHandled](EventPtr event) {
        eventHandled = true;
        EXPECT_EQ(event->getType(), EventType::Game);
    };

    manager.addEventHandler(EventType::Game, handler);
    auto event = std::make_shared<Event>(EventType::Game);
    manager.queueEvent(event);

    // Process events
    manager.processEvents();
    EXPECT_TRUE(eventHandled);
}

TEST(EventManagerTest, EventPriorities) {
    auto& manager = EventManager::getInstance();
    std::vector<EventType> processOrder;

    // Add handlers with different priorities
    auto handler = [&processOrder](EventPtr event) {
        processOrder.push_back(event->getType());
    };

    manager.addEventHandler(EventType::Game, handler, EventPriority::Normal);
    manager.addEventHandler(EventType::Combat, handler, EventPriority::High);
    manager.addEventHandler(EventType::System, handler, EventPriority::Critical);

    // Queue events
    manager.queueEvent(std::make_shared<Event>(EventType::Game));
    manager.queueEvent(std::make_shared<Event>(EventType::Combat));
    manager.queueEvent(std::make_shared<Event>(EventType::System));

    // Process events and check order
    manager.processEvents();
    EXPECT_EQ(processOrder[0], EventType::System);
    EXPECT_EQ(processOrder[1], EventType::Combat);
    EXPECT_EQ(processOrder[2], EventType::Game);
}

TEST(EventManagerTest, EventFiltering) {
    auto& manager = EventManager::getInstance();

    // Create filter
    EventFilter filter{
        {EventType::Combat},
        {},
        {},
        EventPriority::High,
        false
    };

    manager.addEventFilter(filter);

    // Create events
    auto normalEvent = std::make_shared<Event>(EventType::Combat, EventPriority::Normal);
    auto highEvent = std::make_shared<Event>(EventType::Combat, EventPriority::High);

    // Check filtering
    EXPECT_FALSE(manager.passesFilter(normalEvent, filter));
    EXPECT_TRUE(manager.passesFilter(highEvent, filter));
}
```

## Next Steps

1. Add advanced features:

   - Event replay
   - Event compression
   - Event prediction
   - Event validation

2. Implement event tools:

   - Event viewer
   - Event debugger
   - Event profiler
   - Event analyzer

3. Create event systems:

   - Network events
   - Physics events
   - AI events
   - UI events

4. Add optimization features:
   - Event pooling
   - Handler pooling
   - Queue optimization
   - Thread optimization

## Additional Resources

- [Sector Generation Tutorial](./sector_generation.md)
- [State Management Tutorial](./state_management.md)
