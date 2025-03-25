#pragma once

namespace void_contingency {
namespace core {

class Component {
public:
    virtual ~Component() = default;
    virtual void update(float deltaTime) = 0;
};

} // namespace core
} // namespace void_contingency