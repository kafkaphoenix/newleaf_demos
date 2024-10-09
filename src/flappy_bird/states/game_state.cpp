#include "game_state.h"

#include <newleaf/application/application.h>
#include <newleaf/assets/assets_manager.h>
#include <newleaf/logging/log_manager.h>
#include <newleaf/scene/scene_manager.h>
#include <newleaf/serializers/sSettings.h>
#include <newleaf/settings/settings_manager.h>
#include <newleaf/state/states_manager.h>
#include <newleaf/systems/graphics/sRender.h>
#include <newleaf/systems/meta/sDelete.h>
#include <newleaf/systems/world/sTime.h>

#include "dispatchers/game/app_dispatcher.h"
#include "dispatchers/meta/input_dispatcher.h"
#include "dispatchers/meta/window_dispatcher.h"
#include "layers/game/completed_overlay.h"
#include "layers/game/game_layer.h"
#include "layers/game/gameover_overlay.h"
#include "layers/game/pause_overlay.h"
#include "layers/game/ready_overlay.h"
#include "systems/animation/sAnimation.h"
#include "systems/physics/sMovement.h"

namespace fb {

GameState::GameState() : State("game_state") {}

void GameState::on_attach() {
  auto& app = nl::Application::get();
  auto& scene_manager = app.get_scene_manager();
  auto& registry = scene_manager.get_registry();
  auto& states_manager = app.get_states_manager();

  states_manager.push_layer(GameLayer::create());
  states_manager.push_overlay(ReadyOverlay::create(), true);
  states_manager.push_overlay(PauseOverlay::create(), false);
  states_manager.push_overlay(CompletedOverlay::create(), false);
  states_manager.push_overlay(GameoverOverlay::create(), false);

  scene_manager.register_system("delete_system",
                                std::make_unique<nl::DeleteSystem>(-100));
  scene_manager.register_system("render_system",
                                std::make_unique<nl::RenderSystem>(100));
  scene_manager.register_system("time_system",
                                std::make_unique<nl::TimeSystem>(0));
  scene_manager.register_system("animation_system",
                                std::make_unique<AnimationSystem>(7));
  // TODO background system priority 3

  scene_manager.create_entity("meta", "game_state", "game_state");
}

void GameState::on_detach() {
  auto& app = nl::Application::get();
  auto& scene_manager = app.get_scene_manager();

  APP_INFO("saving settings...");
  nl::save_settings(app.get_settings_manager(),
                    nl::get_default_roaming_path("FlappyBird"));

  app.get_states_manager().get_current_state().clear_layers();

  // TODO clear entities from systems and time? should be done in themselves
  scene_manager.clear_systems();

  auto game_state = scene_manager.get_entity("game_state");
  scene_manager.delete_entity(game_state);
}

void GameState::on_update(const nl::Time& ts) {
  auto& app = nl::Application::get();
  auto& settings_manager = app.get_settings_manager();
  auto& scene_manager = app.get_scene_manager();

  if (settings_manager.reload_scene) {
    // TODO should be done in the game layer?
    if (scene_manager.get_active_scene() not_eq settings_manager.active_scene) {
      app.get_states_manager().get_current_state().clear_layers();
      scene_manager.clear_scene();
      app.get_assets_manager().clear();
      scene_manager.create_scene(settings_manager.active_scene,
                                 settings_manager.active_scene_path);
      settings_manager.reload_scene = false;
      on_attach();
    }
  }

  for (auto it = m_layers_manager->begin(); it not_eq m_layers_manager->end();
       ++it) {
    if ((*it)->is_enabled()) {
      (*it)->on_update(ts);
    }
  }
}

void GameState::on_event(nl::Event& e) {
  auto& registry = nl::Application::get().get_scene_manager().get_registry();

  window_dispatcher(e, registry);
  input_dispatcher(e, registry);

  app_dispatcher(e, registry);

  if (e.is_handled) {
    return;
  }

  for (auto it = m_layers_manager->rbegin(); it not_eq m_layers_manager->rend();
       ++it) {
    if ((*it)->is_enabled()) {
      (*it)->on_event(e);
    }
    // to avoid event popping state invalidating the layers iterator
    if (e.is_handled) {
      break;
    }
  }
}

std::unique_ptr<nl::State> GameState::create() {
  return std::make_unique<GameState>();
}
}