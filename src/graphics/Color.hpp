#pragma once
#include <SDL.h>
#include <SDL2/SDL.h>

namespace void_contingency {
namespace graphics {

struct Color {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;

    // Default constructor (black)
    Color() : r(0), g(0), b(0), a(255) {}

    // Constructor with RGB values (alpha defaults to 255)
    Color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = 255)
        : r(red), g(green), b(blue), a(alpha) {}

    // Common colors
    static const Color Black;
    static const Color White;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Yellow;
    static const Color Magenta;
    static const Color Cyan;
    static const Color Transparent;
};

}  // namespace graphics
}  // namespace void_contingency