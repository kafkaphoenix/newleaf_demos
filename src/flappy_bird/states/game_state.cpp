#include "game_state.h"

#include <newleaf/application/application.h>
#include <newleaf/logging/log_manager.h>
#include <newleaf/scene/scene_manager.h>
#include <newleaf/serializers/sSettings.h>
#include <newleaf/settings/settings_manager.h>
#include <newleaf/state/states_manager.h>
#include <newleaf/systems/graphics/sRender.h>
#include <newleaf/systems/meta/sDelete.h>
#include <newleaf/systems/world/sTime.h>

#include "handlers/game/app_handler.h"
#include "handlers/meta/input_handler.h"
#include "handlers/meta/window_handler.h"
#include "layers/game/completed_overlay.h"
#include "layers/game/game_layer.h"
#include "layers/game/gameover_overlay.h"
#include "layers/game/pause_overlay.h"
#include "layers/game/ready_overlay.h"
#include "systems/animation/sAnimation.h"
#include "systems/physics/sBirdMovement.h"

namespace fb {

GameState::GameState() : State("game_state") {}

void GameState::on_attach() {
  auto& app = nl::Application::get();
  auto& scene_manager = app.get_scene_manager();
  auto& states_manager = app.get_states_manager();
  auto& settings_manager = app.get_settings_manager();
  if (settings_manager.reload_scene) {
    // TODO move to loading screen state, this is to prevent incorrect reload on first load
    // maybe settings from debugger like pause should be restarted? to avoid unpausing in debugger
    // breaking the game
    settings_manager.reload_scene = false;
  }

  states_manager.push_layer(GameLayer::create());
  // TODO move loading logic from overlay ready to loading state
  states_manager.push_overlay(ReadyOverlay::create(), true);
  states_manager.push_overlay(PauseOverlay::create(), false);
  states_manager.push_overlay(CompletedOverlay::create(), false);
  states_manager.push_overlay(GameoverOverlay::create(), false);

  scene_manager.register_system("delete_system", std::make_unique<nl::DeleteSystem>(-100));
  scene_manager.register_system("render_system", std::make_unique<nl::RenderSystem>(100));
  scene_manager.register_system("time_system", std::make_unique<nl::TimeSystem>(0));
  scene_manager.register_system("animation_system", std::make_unique<AnimationSystem>(7));
  // TODO background system priority 3

  scene_manager.create_entity("meta", "game_state", "game_state");
}

void GameState::on_detach() {
  auto& app = nl::Application::get();
  auto& scene_manager = app.get_scene_manager();

  // TODO this should be ui and remove import
  APP_INFO("saving settings");
  nl::save_settings(app.get_settings_manager(), nl::get_default_roaming_path("FlappyBird"));

  app.get_states_manager().clear_layers();

  scene_manager.clear_systems();

  scene_manager.delete_entity("game_state");
}

void GameState::on_update(const nl::Time& ts) {
  for (auto it = m_layers_manager->begin(); it not_eq m_layers_manager->end(); ++it) {
    if ((*it)->is_enabled()) {
      (*it)->on_update(ts);
    }
  }
}

void GameState::on_event(nl::Event& e) {
  auto& registry = nl::Application::get().get_scene_manager().get_registry();

  window_handler(e, registry);
  input_handler(e, registry);

  app_handler(e, registry);

  if (e.is_handled) {
    return;
  }

  for (auto it = m_layers_manager->rbegin(); it not_eq m_layers_manager->rend(); ++it) {
    if ((*it)->is_enabled()) {
      (*it)->on_event(e);
    }
    // to avoid event popping state invalidating the layers iterator
    if (e.is_handled) {
      break;
    }
  }
}

std::unique_ptr<nl::State> GameState::create() { return std::make_unique<GameState>(); }
}