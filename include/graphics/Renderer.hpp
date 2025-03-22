#pragma once
#include <SDL.h>
#include <memory>
#include <vector>

namespace void_contingency {
namespace graphics {

// Color structure for rendering
struct Color {
    Uint8 r, g, b, a;
    Color(Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 255) : r(r), g(g), b(b), a(a) {}
};

class Renderer {
public:
    static Renderer& get_instance();

    // System lifecycle methods
    void initialize(SDL_Window* window);
    void shutdown();

    // Rendering methods
    void clear();
    void present();
    void set_draw_color(const Color& color);
    void draw_rect(const SDL_Rect& rect);
    void fill_rect(const SDL_Rect& rect);
    void draw_line(int x1, int y1, int x2, int y2);

    // Access SDL renderer
    SDL_Renderer* get_sdl_renderer() const {
        return renderer_;
    }

private:
    Renderer() = default;
    ~Renderer() = default;

    SDL_Renderer* renderer_ = nullptr;  // SDL renderer handle
};

}  // namespace graphics
}  // namespace void_contingency