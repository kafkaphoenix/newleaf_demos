#pragma once

#include <entt/entt.hpp>
#include <newleaf/systems/system.h>
#include <newleaf/utils/time.h>

namespace fb {

class ScoreSystem : public nl::System {
  public:
    ScoreSystem(int priority) : nl::System(priority) {}
    ~ScoreSystem() override final;

    void init(entt::registry& registry) override final;
};

}
