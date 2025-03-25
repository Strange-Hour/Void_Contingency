#pragma once

#include "Vector2f.hpp"

namespace void_contingency {
namespace core {

struct Transform {
    Vector2f position{0.0f, 0.0f};
    float rotation{0.0f};
    Vector2f scale{1.0f, 1.0f};
};

} // namespace core
} // namespace void_contingency