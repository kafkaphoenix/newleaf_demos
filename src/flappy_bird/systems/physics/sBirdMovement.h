#pragma once

#include <entt/entt.hpp>
#include <newleaf/systems/system.h>
#include <newleaf/utils/time.h>

namespace fb {

class BirdMovementSystem : public nl::System {
  public:
    BirdMovementSystem(int priority) : nl::System(priority) {}

    void update(entt::registry& registry, const nl::Time& ts) override final;
};

}
