#include "gameover_overlay.h"

#include <newleaf/application/application.h>
#include <newleaf/components/graphics/cShaderProgram.h>
#include <newleaf/components/graphics/cTexture.h>
#include <newleaf/components/graphics/cTextureAtlas.h>
#include <newleaf/components/physics/cTransform.h>
#include <newleaf/scene/scene_manager.h>
#include <newleaf/settings/settings_manager.h>
#include <newleaf/state/states_manager.h>

#include "states/menu_state.h"

namespace fb {

GameoverOverlay::GameoverOverlay() : Layer("gameover_overlay") {}

void GameoverOverlay::on_attach() {
  auto& app = nl::Application::get();
  auto& scene_manager = app.get_scene_manager();
  auto& registry = scene_manager.get_registry();

  app.pause(true);

  auto bird = scene_manager.get_entity("bird");
  registry.get<nl::CShaderProgram>(bird).visible = false;

  auto gameover = scene_manager.create_entity("scene", "text", "gameover");
  registry.get<nl::CTransform>(gameover).position.y = 0.3;
  registry.get<nl::CTexture>(gameover).reload_textures({"gameover"});

  auto restart = scene_manager.create_entity("scene", "buttons", "restart");
  registry.get<nl::CTransform>(restart).position.y = -0.2;
  registry.get<nl::CTextureAtlas>(restart).index = 3;

  auto menu = scene_manager.create_entity("scene", "buttons", "menu");
  registry.get<nl::CTransform>(menu).position.y = -0.4;
  registry.get<nl::CTextureAtlas>(menu).index = 8;

  app.get_render_manager().reorder();
}

void GameoverOverlay::on_detach() {
  auto& app = nl::Application::get();
  auto& scene_manager = app.get_scene_manager();

  scene_manager.delete_entity("gameover");
  scene_manager.delete_entity("restart");
  scene_manager.delete_entity("menu");

  app.pause(false);
}

void GameoverOverlay::on_event(nl::Event& e) {
  nl::EventDispatcher dispatcher(e);
  dispatcher.dispatch<nl::KeyPressedEvent>(BIND_EVENT(on_key_pressed));
}

bool GameoverOverlay::on_key_pressed(nl::KeyPressedEvent& e) {
  // repeat event if key is held down
  if (e.is_repeating())
    return false;

  auto& app = nl::Application::get();

  if (e.get_key() == nl::Key::R) {
    m_enabled = false;
    on_detach();
    app.get_settings_manager().reload_scene = true;
    return true;
  } else if (e.get_key() == nl::Key::M) {
    app.get_states_manager().pop_state("game_state");
    return true;
  } else if (e.get_key() == nl::Key::P) {
    return true;
  }

  return false;
}

std::unique_ptr<nl::Layer> GameoverOverlay::create() { return std::make_unique<GameoverOverlay>(); }
}