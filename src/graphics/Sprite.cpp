#include "Sprite.hpp"
// Try different include paths for SDL_image.h
#ifdef SDL_IMAGE_USE_COMMON_BACKEND
#include "SDL_image.h"  // Try direct include
#else
// Try multiple possible paths for SDL_image.h
#if __has_include("SDL2/SDL_image.h")
#include "SDL2/SDL_image.h"
#elif __has_include(<SDL2/SDL_image.h>)
#include <SDL2/SDL_image.h>
#elif __has_include("SDL_image.h")
#include "SDL_image.h"
#elif __has_include(<SDL_image.h>)
#include <SDL_image.h>
#else
// If all else fails, use a relative path
#include "../../../build/_deps/sdl2_image-src/SDL_image.h"
#endif
#endif
#include <stdexcept>
#include "Renderer.hpp"

namespace void_contingency {
namespace graphics {

Sprite::Sprite(SDL_Texture* texture, const SDL_Rect* src_rect)
    : texture_(texture), owns_texture_(false) {
    if (!texture_) {
        throw std::runtime_error("Cannot create sprite with null texture");
    }

    // Set default source rectangle to entire texture if none provided
    if (src_rect) {
        source_rect_ = *src_rect;
    } else {
        int width, height;
        SDL_QueryTexture(texture_, nullptr, nullptr, &width, &height);
        source_rect_ = {0, 0, width, height};
    }
}

Sprite::~Sprite() {
    if (owns_texture_ && texture_) {
        SDL_DestroyTexture(texture_);
    }
}

std::shared_ptr<Sprite> Sprite::load_from_file(const std::string& file_path) {
    SDL_Surface* surface = IMG_Load(file_path.c_str());
    if (!surface) {
        throw std::runtime_error("Failed to load image: " + std::string(IMG_GetError()));
    }

    SDL_Texture* texture =
        SDL_CreateTextureFromSurface(Renderer::get_instance().get_sdl_renderer(), surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        throw std::runtime_error("Failed to create texture: " + std::string(SDL_GetError()));
    }

    auto sprite = std::make_shared<Sprite>(texture);
    sprite->owns_texture_ = true;
    return sprite;
}

void Sprite::render(int x, int y, double angle, SDL_RendererFlip flip) {
    SDL_Rect dest_rect = {x, y, source_rect_.w, source_rect_.h};
    SDL_RenderCopyEx(Renderer::get_instance().get_sdl_renderer(), texture_, &source_rect_,
                     &dest_rect, angle, nullptr, flip);
}

void Sprite::set_source_rect(const SDL_Rect& rect) {
    source_rect_ = rect;
}

void Sprite::get_dimensions(int& width, int& height) const {
    width = source_rect_.w;
    height = source_rect_.h;
}

}  // namespace graphics
}  // namespace void_contingency