#include "game_state.h"

#include <newleaf/application/application.h>
#include <newleaf/logging/log_manager.h>
#include <newleaf/serializers/sSettings.h>
#include <newleaf/settings/settings_manager.h>
#include <newleaf/systems/graphics/sRender.h>
#include <newleaf/systems/meta/sDelete.h>
#include <newleaf/systems/world/sTime.h>

#include <handlers/input_handler.h>
#include <handlers/window_handler.h>
#include <layers/game/game_layer.h>
#include <layers/game/imgui_layer.h>
#include <layers/game/pause_overlay.h>
#include <systems/physics/sPlayerMovement.h>

namespace spz {

GameState::GameState() : State("game_state") {}

void GameState::on_attach() {
  auto& app = nl::Application::get();
  auto& scene_manager = app.get_scene_manager();
  auto& states_manager = app.get_states_manager();

  create_layers(states_manager);
  register_systems(scene_manager);
}

void GameState::create_layers(nl::StatesManager& states_manager) {
  states_manager.push_layer(GameLayer::create());
  states_manager.push_layer(ImGuiLayer::create());
  states_manager.push_overlay(PauseOverlay::create(), false);
}

// these are global systems that should be present in all scenes, they should be
// registered and unregistered with the menu scene or game if there is not menu
// TODO maybe they should be back in the engine?
void GameState::register_systems(nl::SceneManager& scene_manager) {
  scene_manager.register_system("delete_system", std::make_unique<nl::DeleteSystem>(-100));
  scene_manager.register_system("render_system", std::make_unique<nl::RenderSystem>(100));
}

void GameState::unregister_systems(nl::SceneManager& scene_manager) {
  scene_manager.unregister_system("delete_system");
  scene_manager.unregister_system("render_system");
}

void GameState::on_detach() {
  auto& app = nl::Application::get();
  auto& scene_manager = app.get_scene_manager();

  // TODO: this should be UI saving icon and delete import
  APP_INFO("saving settings");
  nl::save_settings(app.get_settings_manager(), nl::get_default_roaming_path(app.get_name()));

  app.get_states_manager().clear_layers();

  unregister_systems(scene_manager);
}

void GameState::on_update(const nl::Time& ts) {
  for (auto it = m_layers_manager->begin(); it not_eq m_layers_manager->end(); ++it) {
    if ((*it)->is_enabled()) {
      (*it)->on_update(ts);
    }
  }
}

void GameState::on_imgui_update() {
  for (auto it = m_layers_manager->begin(); it not_eq m_layers_manager->end(); ++it) {
    if ((*it)->is_enabled()) {
      (*it)->on_imgui_update();
    }
  }
}

void GameState::on_event(nl::Event& e) {
  auto& registry = nl::Application::get().get_scene_manager().get_registry();

  window_handler(e, registry);
  input_handler(e, registry);

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