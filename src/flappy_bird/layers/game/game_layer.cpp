#include "game_layer.h"

#include <newleaf/application/application.h>
#include <newleaf/scene/scene_manager.h>
#include <newleaf/settings/settings_manager.h>
#include <newleaf/state/states_manager.h>
#include <newleaf/systems/world/sTime.h>

namespace fb {

GameLayer::GameLayer() : Layer("game_layer") {}

void GameLayer::on_detach() {
  auto& scene_manager = nl::Application::get().get_scene_manager();

  scene_manager.unregister_system("pipes_system");
  scene_manager.unregister_system("coin_system");
  scene_manager.unregister_system("timer_system");
  scene_manager.unregister_system("collision_system");
  scene_manager.unregister_system("gravity_system");
  scene_manager.unregister_system("score_system");
  scene_manager.unregister_system("movement_system");
  scene_manager.unregister_system("time_system");

  entt::entity bird = scene_manager.get_entity("bird");
  scene_manager.delete_entity(bird);

  entt::entity background_day = scene_manager.get_entity("background_day");
  scene_manager.delete_entity(background_day);

  entt::entity ground = scene_manager.get_entity("ground");
  scene_manager.delete_entity(ground);
}

void GameLayer::on_update(const nl::Time& ts) {
  auto& app = nl::Application::get();
  auto& settings_manager = app.get_settings_manager();
  if (settings_manager.reload_scene) {
    // TODO move to loading screen state
    auto& scene_manager = app.get_scene_manager();
    scene_manager.reload_scene(settings_manager.reload_prototypes);
    scene_manager.unregister_system("pipes_system");
    scene_manager.unregister_system("coin_system");
    scene_manager.unregister_system("timer_system");
    scene_manager.unregister_system("collision_system");
    scene_manager.unregister_system("gravity_system");
    scene_manager.unregister_system("score_system");
    scene_manager.unregister_system("movement_system");
    scene_manager.unregister_system("time_system");
    scene_manager.register_system("time_system",
                                  std::make_unique<nl::TimeSystem>(0));

    scene_manager.create_entity("meta", "camera", "scene_camera");
    scene_manager.create_entity("meta", "game_state", "game_state");
    app.get_states_manager().enable_overlay("ready_overlay");

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

std::unique_ptr<nl::Layer> GameLayer::create() {
  return std::make_unique<GameLayer>();
}
}