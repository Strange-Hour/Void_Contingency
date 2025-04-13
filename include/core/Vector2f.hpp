#pragma once

#include <cmath>

namespace void_contingency {
namespace core {

/**
 * A 2D vector class with float components
 */
class Vector2f {
public:
    // Components
    float x;
    float y;

    // Constructors
    Vector2f() : x(0.0f), y(0.0f) {}
    Vector2f(float x, float y) : x(x), y(y) {}

    // Vector operations
    Vector2f operator+(const Vector2f& other) const {
        return Vector2f(x + other.x, y + other.y);
    }

    Vector2f operator-(const Vector2f& other) const {
        return Vector2f(x - other.x, y - other.y);
    }

    Vector2f operator*(float scalar) const {
        return Vector2f(x * scalar, y * scalar);
    }

    Vector2f operator/(float scalar) const {
        return Vector2f(x / scalar, y / scalar);
    }

    // Compound assignment operators
    Vector2f& operator+=(const Vector2f& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2f& operator-=(const Vector2f& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vector2f& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vector2f& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    // Comparison operators
    bool operator==(const Vector2f& other) const {
        return (x == other.x) && (y == other.y);
    }

    bool operator!=(const Vector2f& other) const {
        return !(*this == other);
    }

    // Vector length operations
    float length() const {
        return std::sqrt(x * x + y * y);
    }

    float lengthSquared() const {
        return x * x + y * y;
    }

    // Normalization
    Vector2f normalized() const {
        float len = length();
        if (len > 0.0f) {
            return Vector2f(x / len, y / len);
        }
        return *this;
    }

    // Dot product
    float dot(const Vector2f& other) const {
        return x * other.x + y * other.y;
    }
};

// Free-function operators
inline Vector2f operator*(float scalar, const Vector2f& vec) {
    return vec * scalar;
}

} // namespace core

// Allow usage of Vector2f directly without namespace qualification in game code
using core::Vector2f;

} // namespace void_contingency 