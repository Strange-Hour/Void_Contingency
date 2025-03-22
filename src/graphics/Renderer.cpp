#include "graphics/Renderer.hpp"
#include <iostream>

namespace void_contingency {
namespace graphics {

// Singleton instance access
Renderer& Renderer::get_instance() {
    static Renderer instance;
    return instance;
}

// Initialize SDL renderer
void Renderer::initialize(SDL_Window* window) {
    renderer_ =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!renderer_) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return;
    }
}

// Clean up SDL renderer
void Renderer::shutdown() {
    if (renderer_) {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }
}

// Clear the renderer
void Renderer::clear() {
    SDL_RenderClear(renderer_);
}

// Present the rendered frame
void Renderer::present() {
    SDL_RenderPresent(renderer_);
}

// Set the current drawing color
void Renderer::set_draw_color(const Color& color) {
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
}

// Draw a rectangle outline
void Renderer::draw_rect(const SDL_Rect& rect) {
    SDL_RenderDrawRect(renderer_, &rect);
}

// Draw a filled rectangle
void Renderer::fill_rect(const SDL_Rect& rect) {
    SDL_RenderFillRect(renderer_, &rect);
}

// Draw a line
void Renderer::draw_line(int x1, int y1, int x2, int y2) {
    SDL_RenderDrawLine(renderer_, x1, y1, x2, y2);
}

}  // namespace graphics
}  // namespace void_contingency