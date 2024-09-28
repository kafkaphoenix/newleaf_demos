#pragma once

#include <entt/entt.hpp>
#include <newleaf/systems/system.h>
#include <newleaf/utils/time.h>

namespace fb {

class TimerSystem : public nl::System {
  public:
    TimerSystem(int priority) : nl::System(priority) {}
    ~TimerSystem() override final;

    void init(entt::registry& registry) override final;
};

}
