#pragma once

#include <entt/entt.hpp>
#include <newleaf/application/application.h>
#include <newleaf/events/app_event.h>
#include <newleaf/events/event.h>

#include "on_coin_collected.h"
#include "on_death.h"
#include "on_level_completed.h"
#include "on_ready.h"
#include "on_timer_ticked.h"

namespace fb {

inline bool on_game_tick(nl::AppTickEvent& e, entt::registry& registry) {
  on_timer_ticked(registry);
  return true;
}

inline bool on_game_update(nl::AppUpdateEvent& e, entt::registry& registry) {
  if (e.get_dispatcher_target() == "on_coin_collected") {
    on_coin_collected(registry);
    return true;
  } else if (e.get_dispatcher_target() == "on_death") {
    on_death(registry);
    return true;
  } else if (e.get_dispatcher_target() == "on_ready") {
    on_ready(registry);
    return true;
  } else if (e.get_dispatcher_target() == "on_level_completed") {
    on_level_completed(registry);
    return true;
  }

  return false;
}

inline bool on_game_render(nl::AppRenderEvent& e) { return true; }

inline void app_dispatcher(nl::Event& e, entt::registry& registry) {
  nl::EventDispatcher dispatcher(e);

  dispatcher.dispatch<nl::AppTickEvent>(
    BIND_STATIC_EVENT(on_game_tick, registry));
  dispatcher.dispatch<nl::AppUpdateEvent>(
    BIND_STATIC_EVENT(on_game_update, registry));
  dispatcher.dispatch<nl::AppRenderEvent>(BIND_STATIC_EVENT(on_game_render));
}
}