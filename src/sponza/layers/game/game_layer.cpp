#include "game_layer.h"

#include <newleaf/application/application.h>
#include <newleaf/settings/settings_manager.h>
#include <newleaf/state/states_manager.h>
#include <newleaf/systems/world/sFog.h>
#include <newleaf/systems/world/sSkybox.h>
#include <newleaf/systems/world/sTime.h>

#include <systems/physics/sPlayerMovement.h>

namespace spz {

GameLayer::GameLayer() : Layer("game_layer") {
  auto& app = nl::Application::get();
  const auto& settings_manager = app.get_settings_manager();
  // TODO move to loading screen state
  app.get_scene_manager().create_scene(settings_manager.active_scene, settings_manager.active_scene_path);
}

void GameLayer::on_attach() {
  auto& scene_manager = nl::Application::get().get_scene_manager();
  register_systems(scene_manager);
  create_entities(scene_manager);
}

void GameLayer::register_systems(nl::SceneManager& scene_manager) {
  scene_manager.register_system("time_system", std::make_unique<nl::TimeSystem>(0));
  scene_manager.register_system("skybox_system", std::make_unique<nl::SkyboxSystem>(1));
  scene_manager.register_system("fog_system", std::make_unique<nl::FogSystem>(2));
  scene_manager.register_system("movement_system", std::make_unique<PlayerMovementSystem>(3));
}

void GameLayer::unregister_systems(nl::SceneManager& scene_manager) {
  scene_manager.unregister_system("time_system");
  scene_manager.unregister_system("movement_system");
  scene_manager.unregister_system("fog_system");
  scene_manager.unregister_system("skybox_system");
}

void GameLayer::create_entities(nl::SceneManager& scene_manager) {
  scene_manager.create_entity("structures", "building", "building");
  scene_manager.create_entity("creatures", "player", "player");
  scene_manager.create_entity("systems", "skybox", "skybox");
  scene_manager.create_entity("systems", "fbo", "fbo");
  scene_manager.create_entity("systems", "clock", "clock");
}

void GameLayer::destroy_entities(nl::SceneManager& scene_manager) {
  scene_manager.delete_entity("building");
  scene_manager.delete_entity("player");
  scene_manager.delete_entity("skybox");
  scene_manager.delete_entity("fbo");
  scene_manager.delete_entity("clock");
}

void GameLayer::on_detach() {
  auto& scene_manager = nl::Application::get().get_scene_manager();
  unregister_systems(scene_manager);
  destroy_entities(scene_manager);
}

void GameLayer::on_update(const nl::Time& ts) {
  auto& app = nl::Application::get();
  auto& settings_manager = app.get_settings_manager();
  if (settings_manager.reload_scene) {
    // TODO move to loading screen state
    auto& scene_manager = app.get_scene_manager();
    scene_manager.reload_scene(settings_manager.reload_prototypes);
    create_entities(scene_manager);
    settings_manager.reload_scene = false;
  }
}

void GameLayer::on_event(nl::Event& e) {
  nl::EventDispatcher dispatcher(e);
  dispatcher.dispatch<nl::KeyPressedEvent>(BIND_EVENT(on_key_pressed));
}

bool GameLayer::on_key_pressed(nl::KeyPressedEvent& e) {
  // repeat event if key is held down
  if (e.is_repeating())
    return false;

  auto& app = nl::Application::get();
  if (e.get_key() == nl::Key::R) {
    app.get_settings_manager().reload_scene = true;
    return true;
  } else if (e.get_key() == nl::Key::P) {
    app.get_states_manager().enable_overlay("pause_overlay");
    return true;
  }

  return false;
}

std::unique_ptr<nl::Layer> GameLayer::create() { return std::make_unique<GameLayer>(); }
}