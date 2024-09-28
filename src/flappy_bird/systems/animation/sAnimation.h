#pragma once

#include <entt/entt.hpp>
#include <newleaf/systems/system.h>
#include <newleaf/utils/time.h>

namespace fb {

class AnimationSystem : public nl::System {
  public:
    AnimationSystem(int priority) : nl::System(priority) {}

    void update(entt::registry& registry, const nl::Time& ts) override final;
};

}
