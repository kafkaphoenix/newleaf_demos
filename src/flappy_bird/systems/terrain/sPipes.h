#pragma once

#include <entt/entt.hpp>
#include <newleaf/systems/system.h>
#include <newleaf/utils/time.h>

namespace fb {

class PipesSystem : public nl::System {
  public:
    PipesSystem(int priority) : nl::System(priority) {}
    ~PipesSystem() override final;

    void init(entt::registry& registry) override final;
    void update(entt::registry& registry, const nl::Time& ts) override final;
};

}
