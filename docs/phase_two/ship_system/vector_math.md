# Vector Math for Ship Systems

## Navigation

- Previous: [Basic Ship Class](./basic_ship_class.md)
- Next: [Component System Tutorial](./component_system.md)

## What We're Building

We're creating a 2D vector class (`Vector2f`) that will serve as the foundation for all position, movement, and physics calculations in our game.

## Game Development Concepts Covered

1. **Vector Mathematics**: How 2D vectors are used in game development
2. **Operator Overloading**: Using C++ operator overloading for intuitive vector operations
3. **Normalization**: Creating unit vectors for direction calculations
4. **Dot Products**: Using dot products for angle and projection calculations
5. **Vector Operations**: Addition, subtraction, scaling, and other common vector operations

## Why This Implementation?

The tutorial uses this approach because:

1. **Efficiency**: Optimized for 2D game development
2. **Intuitiveness**: Operator overloading makes vector math code read naturally
3. **Reusability**: A single vector class that can be used throughout the codebase
4. **Precision**: Float-based for game physics calculations
5. **Simplicity**: Focused implementation with only the necessary functionality

## Implementation Steps

### 1. Create the Vector2f Header File

Create a new file `include/core/Vector2f.hpp`:

```cpp
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
```

## Key Concepts Explained

### Vector Basics

- A vector represents both direction and magnitude
- In 2D space, a vector has x and y components
- Vectors can represent position, velocity, acceleration, forces, etc.

### Vector Operations

- **Addition**: Combining two vectors (e.g., position + velocity)
- **Subtraction**: Finding the difference between vectors (e.g., direction from A to B)
- **Scalar Multiplication**: Scaling a vector (e.g., velocity \* time)
- **Division**: Dividing a vector by a scalar (e.g., normalizing)

### Normalization

- A normalized vector has a length of 1.0 (a unit vector)
- Used to represent direction without magnitude
- Essential for directional calculations

### Dot Product

- The dot product gives a scalar value based on two vectors
- Used to calculate:
  - The angle between vectors
  - Projections of one vector onto another
  - Whether vectors are perpendicular (dot product = 0)

## Usage Examples

### Position and Movement

```cpp
// Set a ship's position
ship.getTransform().position = Vector2f(100.0f, 200.0f);

// Move a ship based on velocity and time
ship.getTransform().position += ship.getVelocity() * deltaTime;
```

### Direction Calculation

```cpp
// Calculate direction from ship to target
Vector2f direction = (target.getTransform().position - ship.getTransform().position).normalized();

// Apply thrust in that direction
ship.setVelocity(ship.getVelocity() + direction * thrust * deltaTime);
```

### Force Application

```cpp
// Calculate force based on direction and magnitude
Vector2f force = direction * magnitude;

// Apply force to velocity
velocity += force * (1.0f / mass) * deltaTime;
```

## Common Pitfalls

1. **Division by Zero**:

   - Always check for zero length before normalizing
   - The implementation handles this automatically

2. **Precision Issues**:

   - Be aware of floating-point precision limitations
   - Use lengthSquared() for comparisons when possible

3. **Performance Considerations**:
   - Square root operations (in length()) are expensive
   - Use lengthSquared() when comparing distances

## Best Practices

1. Use normalized vectors for direction calculations
2. Prefer operator overloading for readability
3. Use the dot product for angle calculations
4. Consider performance implications in tight loops
5. Add equality comparison if needed

## Integration with Transform System

The `Vector2f` class integrates seamlessly with our Transform system:

```cpp
struct Transform {
    Vector2f position{0.0f, 0.0f};
    float rotation{0.0f};
    Vector2f scale{1.0f, 1.0f};

    // Get forward direction vector based on rotation
    Vector2f getForward() const {
        const float radians = rotation * (3.14159265f / 180.0f);
        return Vector2f(std::cos(radians), std::sin(radians));
    }
};
```

## Next Steps

1. Expand with additional vector operations as needed
2. Consider adding cross product for 2D vectors (z-component only)
3. Add comparison operators for equality checks
4. Implement vector interpolation for smooth transitions

## Additional Resources

- [Basic Ship Class](./basic_ship_class.md)
- [Component System Tutorial](./component_system.md)
- [Movement System Tutorial](./movement_system.md)
- [Ship Visualization Tutorial](./visualization.md)
- [Collision System Tutorial](./collision_system.md)
