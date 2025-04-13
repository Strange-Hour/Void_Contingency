#pragma once

#include "Vector2f.hpp"
#include <cmath>

namespace void_contingency {
namespace core {

struct Transform {
    Vector2f position{0.0f, 0.0f};
    float rotation{0.0f};
    Vector2f scale{1.0f, 1.0f};

    // Get forward direction vector based on rotation
    Vector2f getForward() const {
        // Convert degrees to radians and calculate direction vector
        const float radians = rotation * (3.14159265f / 180.0f);
        return Vector2f(std::cos(radians), std::sin(radians));
    }
};

} // namespace core
} // namespace void_contingency