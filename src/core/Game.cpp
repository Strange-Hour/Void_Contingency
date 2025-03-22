#include "core/Game.hpp"
#include <SDL.h>
#include <iostream>
#include "graphics/Renderer.hpp"
#include "input/InputSystem.hpp"
#include "utils/Logger.hpp"

namespace void_contingency {
namespace core {

// Constructor initializes the game state
Game::Game() : is_running_(false) {}

// Destructor ensures proper cleanup
Game::~Game() {
    shutdown();
}

// Initialize game systems and resources
void Game::initialize() {
    utils::Logger::get_instance().log(utils::LogLevel::INFO, "Initializing game...");
    is_running_ = true;

    // Register input callbacks
    input::InputSystem::get_instance().register_key_callback(SDLK_ESCAPE, input::KeyAction::PRESS,
                                                             [this]() { is_running_ = false; });
}

// Main game loop implementation
void Game::run() {
    utils::Logger::get_instance().log(utils::LogLevel::INFO, "Starting game loop");

    while (is_running_) {
        process_input();  // Handle user input first
        update();         // Update game state
        render();         // Render the current frame

        // Add a small delay to prevent CPU overuse
        SDL_Delay(16);  // Approximately 60 FPS
    }
}

// Cleanup and shutdown
void Game::shutdown() {
    utils::Logger::get_instance().log(utils::LogLevel::INFO, "Shutting down game...");
    is_running_ = false;
}

// Input processing
void Game::process_input() {
    input::InputSystem::get_instance().update();
}

// Game state update
void Game::update() {
    // For now, just check if ESC is pressed to exit
    if (input::InputSystem::get_instance().is_key_pressed(SDLK_ESCAPE)) {
        is_running_ = false;
    }
}

// Rendering
void Game::render() {
    // Set the draw color to dark blue
    graphics::Renderer::get_instance().set_draw_color(graphics::Color(25, 25, 76, 255));

    // Clear the screen
    graphics::Renderer::get_instance().clear();

    // TODO: Render game objects here

    // Present the rendered frame
    graphics::Renderer::get_instance().present();
}

}  // namespace core
}  // namespace void_contingency