#pragma once

#include <memory>
#include <vector>
#include <string>
#include "core/Component.hpp"
#include "core/Transform.hpp"

namespace void_contingency {
namespace game {

class Ship {
public:
    Ship(const std::string& name);
    ~Ship() = default;

    // Basic properties
    const std::string& getName() const { return name_; }
    float getHealth() const { return health_; }
    float getMaxHealth() const { return maxHealth_; }
    const Transform& getTransform() const { return transform_; }

    // Movement
    void setVelocity(const Vector2f& velocity) { velocity_ = velocity; }
    const Vector2f& getVelocity() const { return velocity_; }

    // Component management
    void addComponent(std::unique_ptr<Component> component);
    template<typename T>
    T* getComponent() const;

    // State management
    void update(float deltaTime);
    void damage(float amount);
    void heal(float amount);

private:
    std::string name_;
    float health_;
    float maxHealth_;
    Transform transform_;
    Vector2f velocity_;
    std::vector<std::unique_ptr<Component>> components_;
};

} // namespace game
} // namespace void_contingency