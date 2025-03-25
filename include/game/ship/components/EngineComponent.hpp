#pragma once

#include "ShipComponent.hpp"

namespace void_contingency {
namespace game {

class EngineComponent : public ShipComponent {
public:
    EngineComponent();

    void initialize() override;
    void shutdown() override;
    void update(float deltaTime) override;

    void setThrust(float thrust);
    float getThrust() const;
    float getMaxThrust() const;

private:
    float thrust_{0.0f};
    float maxThrust_{100.0f};
    float efficiency_{0.8f};
};

} // namespace game
} // namespace void_contingency