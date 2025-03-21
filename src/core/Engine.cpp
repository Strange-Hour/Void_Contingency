#include "core/Engine.hpp"
#include <iostream>

namespace void_contingency {
namespace core {

Engine& Engine::get_instance() {
    static Engine instance;
    return instance;
}

void Engine::initialize() {
    std::cout << "Initializing engine..." << std::endl;
}

void Engine::shutdown() {
    std::cout << "Shutting down engine..." << std::endl;
}

}  // namespace core
}  // namespace void_contingency