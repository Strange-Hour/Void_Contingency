#include "core/Engine.hpp"
#include "core/Config.hpp"
#include "core/EventManager.hpp"
#include "core/ResourceManager.hpp"
#include "graphics/Renderer.hpp"
#include "input/InputSystem.hpp"
#include "utils/Logger.hpp"

namespace void_contingency {
namespace core {

// Singleton instance access
Engine& Engine::get_instance() {
    static Engine instance;
    return instance;
}

// Initialize all core systems
void Engine::initialize() {
    // Initialize logging first for error reporting
    utils::Logger::get_instance().initialize("void_contingency.log");
    utils::Logger::get_instance().log(utils::LogLevel::INFO, "Initializing engine...");

    // Load configuration
    Config::get_instance().load_from_file("config.ini");

    // Initialize input system
    input::InputSystem::get_instance().initialize();

    // Initialize renderer with the window from input system
    graphics::Renderer::get_instance().initialize(input::InputSystem::get_instance().get_window());

    // Subscribe to core events
    EventManager::get_instance().subscribe<GameStartEvent>([](const GameStartEvent&) {
        utils::Logger::get_instance().log(utils::LogLevel::INFO, "Game started");
    });

    EventManager::get_instance().subscribe<GameEndEvent>([](const GameEndEvent&) {
        utils::Logger::get_instance().log(utils::LogLevel::INFO, "Game ended");
    });
}

// Shutdown all core systems
void Engine::shutdown() {
    utils::Logger::get_instance().log(utils::LogLevel::INFO, "Shutting down engine...");

    // Save configuration
    Config::get_instance().save_to_file("config.ini");

    // Shutdown systems in reverse initialization order
    graphics::Renderer::get_instance().shutdown();
    input::InputSystem::get_instance().shutdown();
    ResourceManager::get_instance().unload_all();
    EventManager::get_instance().clear();

    utils::Logger::get_instance().log(utils::LogLevel::INFO, "Engine shutdown complete");
}

}  // namespace core
}  // namespace void_contingency