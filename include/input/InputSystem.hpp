#pragma once
#include <SDL.h>
#include <functional>
#include <unordered_map>

namespace void_contingency {
namespace input {

// Define possible key actions
enum class KeyAction {
    PRESS,    // Key was pressed
    RELEASE,  // Key was released
    REPEAT    // Key is being held down
};

class InputSystem {
public:
    static InputSystem& get_instance();

    // System lifecycle methods
    void initialize();  // Set up SDL and input handling
    void shutdown();    // Clean up SDL resources
    void update();      // Process input events

    // Keyboard input methods
    bool is_key_pressed(SDL_Keycode key) const;
    void register_key_callback(SDL_Keycode key, KeyAction action, std::function<void()> callback);

    // Mouse input methods
    bool is_mouse_button_pressed(Uint8 button) const;
    void get_mouse_position(int& x, int& y) const;

private:
    InputSystem() = default;
    ~InputSystem() = default;

    SDL_Window* window_ = nullptr;                      // SDL window handle
    std::unordered_map<SDL_Keycode, bool> key_states_;  // Current key states
    std::unordered_map<SDL_Keycode, std::vector<std::pair<KeyAction,
                                                          std::function<void()>>>>
        key_callbacks_;  // Key event callbacks
};

}  // namespace input
}  // namespace void_contingency