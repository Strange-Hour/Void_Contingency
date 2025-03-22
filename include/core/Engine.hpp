#pragma once

namespace void_contingency {
namespace core {

class Engine {
public:
    // Singleton pattern implementation
    static Engine& get_instance();

    // Core engine lifecycle methods
    void initialize();  // Sets up engine systems
    void shutdown();    // Cleans up engine resources

    // Prevent copying to maintain singleton pattern
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

private:
    Engine() = default;   // Private constructor
    ~Engine() = default;  // Private destructor
};

}  // namespace core
}  // namespace void_contingency