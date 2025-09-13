#pragma once

#include <entt/entt.hpp>
#include <newleaf/application/application.h>
#include <newleaf/state/states_manager.h>

#include "layers/game/gameover_overlay.h"

namespace fb {

inline void on_death(entt::registry& registry) {
  nl::Application::get().get_states_manager().enable_overlay("gameover_overlay");
}
}