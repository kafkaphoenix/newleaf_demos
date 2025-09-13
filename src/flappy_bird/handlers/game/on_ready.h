#pragma once

#include <entt/entt.hpp>
#include <newleaf/application/application.h>
#include <newleaf/state/states_manager.h>

namespace fb {

inline void on_ready(entt::registry& registry) {
  nl::Application::get().get_states_manager().disable_overlay("ready_overlay");
}
}