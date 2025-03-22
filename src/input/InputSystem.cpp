#include "input/InputSystem.hpp"
#include <iostream>

namespace void_contingency {
namespace input {

// Singleton instance access
InputSystem& InputSystem::get_instance() {
    static InputSystem instance;
    return instance;
}

// Initialize SDL and create window
void InputSystem::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return;
    }

    window_ = SDL_CreateWindow("Void Contingency", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               1280, 720, SDL_WINDOW_SHOWN);

    if (!window_) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return;
    }
}

// Clean up SDL resources
void InputSystem::shutdown() {
    if (window_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
    SDL_Quit();
}

// Process input events
void InputSystem::update() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                // Handle quit event
                break;

            case SDL_KEYDOWN:
                key_states_[event.key.keysym.sym] = true;
                if (key_callbacks_.count(event.key.keysym.sym)) {
                    for (const auto& callback : key_callbacks_[event.key.keysym.sym]) {
                        if (callback.first == KeyAction::PRESS) {
                            callback.second();
                        }
                    }
                }
                break;

            case SDL_KEYUP:
                key_states_[event.key.keysym.sym] = false;
                if (key_callbacks_.count(event.key.keysym.sym)) {
                    for (const auto& callback : key_callbacks_[event.key.keysym.sym]) {
                        if (callback.first == KeyAction::RELEASE) {
                            callback.second();
                        }
                    }
                }
                break;
        }
    }
}

// Check if a key is currently pressed
bool InputSystem::is_key_pressed(SDL_Keycode key) const {
    auto it = key_states_.find(key);
    return it != key_states_.end() && it->second;
}

// Register a callback for a key event
void InputSystem::register_key_callback(SDL_Keycode key, KeyAction action,
                                        std::function<void()> callback) {
    key_callbacks_[key].push_back({action, callback});
}

// Check if a mouse button is pressed
bool InputSystem::is_mouse_button_pressed(Uint8 button) const {
    return (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(button)) != 0;
}

// Get current mouse position
void InputSystem::get_mouse_position(int& x, int& y) const {
    SDL_GetMouseState(&x, &y);
}

}  // namespace input
}  // namespace void_contingency