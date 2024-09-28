#include "ready_overlay.h"

#include <newleaf/application/application.h>
#include <newleaf/components/graphics/cShaderProgram.h>
#include <newleaf/components/graphics/cTextureAtlas.h>
#include <newleaf/components/physics/cTransform.h>
#include <newleaf/scene/scene_manager.h>
#include <newleaf/state/states_manager.h>

#include "systems/items/sCoins.h"
#include "systems/meta/sScore.h"
#include "systems/meta/sTimer.h"
#include "systems/physics/sCollision.h"
#include "systems/physics/sGravity.h"
#include "systems/physics/sMovement.h"
#include "systems/terrain/sPipes.h"

namespace fb {

ReadyOverlay::ReadyOverlay() : Layer("ready_overlay") {}

void ReadyOverlay::on_attach() {
  auto& app = nl::Application::get();
  auto& scene_manager = app.get_scene_manager();
  auto& registry = app.get_scene_manager().get_registry();

  auto bird = scene_manager.get_entity("bird");
  registry.get<nl::CShaderProgram>(bird).visible = true;
  registry.get<nl::CTransform>(bird).position.y = 0.2;

  auto getReady = scene_manager.get_entity("ready");
  registry.get<nl::CShaderProgram>(getReady).visible = true;

  auto countdown = scene_manager.create_entity("scene", "numbers", "countdown");
  registry.get<nl::CTextureAtlas>(countdown).index = 3;
  registry.get<nl::CTransform>(countdown).position.y = 0.45;

  scene_manager.create_entity("meta", "game_state", "game_state");

  app.get_render_manager().reorder();
}

void ReadyOverlay::on_detach() {
  auto& app = nl::Application::get();
  auto& scene_manager = app.get_scene_manager();
  auto& registry = app.get_scene_manager().get_registry();
  entt::entity getReady = scene_manager.get_entity("ready");
  registry.get<nl::CShaderProgram>(getReady).visible = false;
  entt::entity countdown = scene_manager.get_entity("countdown");
  scene_manager.delete_entity(countdown);

  scene_manager.register_system("pipes_system",
                                std::make_unique<PipesSystem>(1));
  scene_manager.register_system("coin_system",
                                std::make_unique<CoinsSystem>(2));
  scene_manager.register_system("timer_system",
                                std::make_unique<TimerSystem>(4));
  scene_manager.register_system("collision_system",
                                std::make_unique<CollisionSystem>(5));
  scene_manager.register_system("gravity_system",
                                std::make_unique<GravitySystem>(6));
  scene_manager.register_system("movement_system",
                                std::make_unique<MovementSystem>(8));
  scene_manager.register_system("score_system",
                                std::make_unique<ScoreSystem>(9));
}

void ReadyOverlay::on_event(nl::Event& e) {
  nl::EventDispatcher dispatcher(e);
  dispatcher.dispatch<nl::KeyPressedEvent>(BIND_EVENT(on_key_pressed));
}

bool ReadyOverlay::on_key_pressed(nl::KeyPressedEvent& e) {
  // repeat event if key is held down
  if (e.is_repeating())
    return false;

  if (e.get_key() == nl::Key::R) {
    return true;
  } else if (e.get_key() == nl::Key::P) {
    return true;
  }

  return false;
}

std::unique_ptr<nl::Layer> ReadyOverlay::create() {
  return std::make_unique<ReadyOverlay>();
}
}