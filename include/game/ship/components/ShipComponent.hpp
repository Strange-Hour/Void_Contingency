#pragma once

#include "core/Component.hpp"
#include "game/ship/Ship.hpp"
#include "ComponentTypes.hpp"

namespace void_contingency {
namespace game {

class ShipComponent : public core::Component {
public:
    ShipComponent(ComponentType type);
    virtual ~ShipComponent() = default;

    // Component interface
    void setShip(Ship* ship) { ship_ = ship; }
    Ship* getShip() const { return ship_; }
    ComponentType getType() const { return type_; }

    // Component lifecycle
    virtual void initialize() = 0;
    virtual void shutdown() = 0;
    virtual void update(float deltaTime) override = 0;

protected:
    Ship* ship_{nullptr};
    ComponentType type_;
};

} // namespace game
} // namespace void_contingency