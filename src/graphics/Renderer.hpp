#pragma once
#include <SDL.h>
#include <SDL2/SDL.h>
#include "Color.hpp"

namespace void_contingency {
namespace graphics {

class Renderer {
public:
    // Singleton access
    static Renderer& get_instance();

    // Initialize the renderer with a window
    void initialize(SDL_Window* window);

    // Clean up resources
    void shutdown();

    // Clear the renderer
    void clear();

    // Present the rendered frame
    void present();

    // Set the current drawing color
    void set_draw_color(const Color& color);

    // Draw a rectangle outline
    void draw_rect(const SDL_Rect& rect);

    // Draw a filled rectangle
    void fill_rect(const SDL_Rect& rect);

    // Draw a line
    void draw_line(int x1, int y1, int x2, int y2);

    // Get the SDL renderer (for internal use)
    SDL_Renderer* get_sdl_renderer() const {
        return renderer_;
    }

private:
    // Private constructor for singleton
    Renderer() : renderer_(nullptr) {}

    // Prevent copying
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    SDL_Renderer* renderer_;
};

}  // namespace graphics
}  // namespace void_contingency