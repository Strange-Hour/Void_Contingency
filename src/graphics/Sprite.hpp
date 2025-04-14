#pragma once
#include <SDL.h>
#include <SDL2/SDL.h>
#include <memory>
#include <string>

namespace void_contingency {
namespace graphics {

class Sprite {
public:
    // Constructor that takes a texture and optional source rectangle
    Sprite(SDL_Texture* texture, const SDL_Rect* src_rect = nullptr);

    // Destructor
    ~Sprite();

    // Load a sprite from a file
    static std::shared_ptr<Sprite> load_from_file(const std::string& file_path);

    // Render the sprite at the specified position
    void render(int x, int y, double angle = 0.0, SDL_RendererFlip flip = SDL_FLIP_NONE);

    // Set the source rectangle (for sprite sheets)
    void set_source_rect(const SDL_Rect& rect);

    // Get the dimensions of the sprite
    void get_dimensions(int& width, int& height) const;

private:
    SDL_Texture* texture_;
    SDL_Rect source_rect_;
    bool owns_texture_;
};

}  // namespace graphics
}  // namespace void_contingency