#include "pause_overlay.h"

#include <newleaf/application/application.h>
#include <newleaf/assets/assets_manager.h>
#include <newleaf/components/graphics/cShaderProgram.h>
#include <newleaf/components/graphics/cTexture.h>
#include <newleaf/components/graphics/cTextureAtlas.h>
#include <newleaf/components/physics/cTransform.h>
#include <newleaf/scene/scene_manager.h>
#include <newleaf/settings/settings_manager.h>
#include <newleaf/state/states_manager.h>

#include "states/menu_state.h"

namespace fb {

PauseOverlay::PauseOverlay() : Layer("pause_overlay") {}

void PauseOverlay::on_attach() {
  auto& app = nl::Application::get();
  auto& scene_manager = app.get_scene_manager();
  auto& registry = scene_manager.get_registry();

  app.pause(true);

  auto bird = scene_manager.get_entity("bird");
  registry.get<nl::CShaderProgram>(bird).visible = false;

  // auto overlay = scene_manager.get_entity("overlay"); // TODO transparency
  // not working

  auto paused = scene_manager.create_entity("scene", "text", "paused");
  registry.get<nl::CTexture>(paused).reload_textures({"paused"});

  auto resume = scene_manager.create_entity("scene", "buttons", "resume");
  registry.get<nl::CTextureAtlas>(resume).index = 4;

  auto restart = scene_manager.create_entity("scene", "buttons", "restart");
  registry.get<nl::CTransform>(restart).position.y = -0.2;
  registry.get<nl::CTextureAtlas>(restart).index = 3;

  auto menu = scene_manager.create_entity("scene", "buttons", "menu");
  registry.get<nl::CTransform>(menu).position.y = -0.4;
  registry.get<nl::CTextureAtlas>(menu).index = 8;

  app.get_render_manager().reorder();
}

void PauseOverlay::on_detach() {
  auto& app = nl::Application::get();
  auto& scene_manager = app.get_scene_manager();
  auto& registry = scene_manager.get_registry();

  // For when we unpause the game
  auto bird = scene_manager.get_entity("bird");
  registry.get<nl::CShaderProgram>(bird).visible = true;

  auto paused = scene_manager.get_entity("paused");
  scene_manager.delete_entity(paused);

  auto resume = scene_manager.get_entity("resume");
  scene_manager.delete_entity(resume);

  auto restart = scene_manager.get_entity("restart");
  scene_manager.delete_entity(restart);

  auto menu = scene_manager.get_entity("menu");
  scene_manager.delete_entity(menu);

  app.pause(false);
}

void PauseOverlay::on_event(nl::Event& e) {
  nl::EventDispatcher dispatcher(e);
  dispatcher.dispatch<nl::KeyPressedEvent>(BIND_EVENT(on_key_pressed));
}

bool PauseOverlay::on_key_pressed(nl::KeyPressedEvent& e) {
  // repeat event if key is held down
  if (e.is_repeating())
    return false;

  auto& app = nl::Application::get();

  if (e.get_key() == nl::Key::P) {
    m_enabled = false;
    on_detach();
    return true;
  } else if (e.get_key() == nl::Key::R) {
    m_enabled = false;
    on_detach();
    app.get_settings_manager().reload_scene = true;
    return true;
  } else if (e.get_key() == nl::Key::M) {
    app.get_states_manager().pop_state("game_state");
    // TODO delete this and import
    if (app.get_states_manager().get_state_index() == 0) {
      app.get_scene_manager().clear_scene();
      app.get_assets_manager().clear();
      app.get_states_manager().push_state(MenuState::create());
    }
    return true;
  }

  return false;
}

std::unique_ptr<nl::Layer> PauseOverlay::create() {
  return std::make_unique<PauseOverlay>();
}
}