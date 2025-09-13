#pragma once

#include <entt/entt.hpp>
#include <newleaf/application/application.h>
#include <newleaf/components/graphics/cTextureAtlas.h>
#include <newleaf/components/meta/cName.h>
#include <newleaf/components/meta/cUUID.h>
#include <newleaf/events/app_event.h>
#include <newleaf/window/windows_manager.h>

#include "components/meta/cTimer.h"

namespace fb {

inline void on_timer_ticked(entt::registry& registry) {
  auto& app = nl::Application::get();
  entt::entity game_state = registry.view<CTimer, nl::CUUID>().front();
  CTimer& timer = registry.get<CTimer>(game_state);
  int left = --timer.left;
  if (timer.max_time - left == 3) {
    app.get_windows_manager().trigger_event(nl::AppUpdateEvent("on_ready"));
  }
  if (left < 0) {
    app.get_windows_manager().trigger_event(nl::AppUpdateEvent("on_level_completed"));
  } else {
    registry.view<nl::CTextureAtlas, nl::CName, nl::CUUID>().each(
      [&](entt::entity e, nl::CTextureAtlas& cTextureAtlas, const nl::CName& cName, const nl::CUUID& cUUID) {
        if (cName.name == "timer_unidades") {
          cTextureAtlas.index = left % 10;
        } else if (cName.name == "timer_decenas") {
          cTextureAtlas.index = (left % 100) / 10;
        } else if (cName.name == "timer_centenas") {
          cTextureAtlas.index = left / 100;
        } else if (cName.name == "countdown") {
          cTextureAtlas.index -= 1;
        }
      });
  }
}
}