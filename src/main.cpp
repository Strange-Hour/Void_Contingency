#include <exception>
#include <iostream>
#include "core/Engine.hpp"
#include "core/Game.hpp"

int main(int argc, char* argv[]) {
    try {
        // Initialize the engine first
        void_contingency::core::Engine::get_instance().initialize();

        // Create and run the game
        void_contingency::core::Game game;
        game.initialize();
        game.run();

        // Cleanup engine resources
        void_contingency::core::Engine::get_instance().shutdown();

        return 0;
    } catch (const std::exception& e) {
        // Handle any uncaught exceptions
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}