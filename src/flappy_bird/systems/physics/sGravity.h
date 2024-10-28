#pragma once

#include <entt/entt.hpp>
#include <newleaf/systems/system.h>
#include <newleaf/utils/time.h>

namespace fb {

class GravitySystem : public nl::System {
  public:
    GravitySystem(int priority) : nl::System(priority) {}

    void update(entt::registry& registry, const nl::Time& ts) override final;
};

}
