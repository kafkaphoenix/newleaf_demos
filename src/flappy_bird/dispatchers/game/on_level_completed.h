#pragma once

#include <entt/entt.hpp>
#include <newleaf/application/application.h>
#include <newleaf/state/states_manager.h>

#include "layers/game/completed_overlay.h"

namespace fb {

inline void on_level_completed(entt::registry& registry) {
  nl::Application::get().get_states_manager().push_overlay(CompletedOverlay::create(), true);
}
}