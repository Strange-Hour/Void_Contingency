#include "core/Game.hpp"
#include <iostream>

namespace void_contingency {
namespace core {

Game::Game() : is_running_(false) {}

Game::~Game() {
    shutdown();
}

void Game::initialize() {
    std::cout << "Initializing game..." << std::endl;
    is_running_ = true;
}

void Game::run() {
    while (is_running_) {
        process_input();
        update();
        render();
    }
}

void Game::shutdown() {
    std::cout << "Shutting down game..." << std::endl;
    is_running_ = false;
}

void Game::process_input() {
    // TODO: Implement input processing
}

void Game::update() {
    // TODO: Implement game update
}

void Game::render() {
    // TODO: Implement rendering
}

}  // namespace core
}  // namespace void_contingency