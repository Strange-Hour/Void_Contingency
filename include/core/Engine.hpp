#pragma once

namespace void_contingency {
namespace core {

class Engine {
public:
    static Engine& get_instance();

    void initialize();
    void shutdown();

    // Prevent copying
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

private:
    Engine() = default;
    ~Engine() = default;
};

}  // namespace core
}  // namespace void_contingency