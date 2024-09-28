#include "completed_overlay.h"

#include <newleaf/application/application.h>
#include <newleaf/assets/assets_manager.h>
#include <newleaf/components/graphics/cShaderProgram.h>
#include <newleaf/components/graphics/cShape.h>
#include <newleaf/components/physics/cTransform.h>
#include <newleaf/scene/scene_manager.h>
#include <newleaf/settings/settings_manager.h>
#include <newleaf/state/states_manager.h>

#include "states/menu_state.h"

namespace fb {

CompletedOverlay::CompletedOverlay() : Layer("completed_overlay") {}

void CompletedOverlay::on_attach() {
  auto& app = nl::Application::get();
  auto& scene_manager = app.get_scene_manager();
  auto& registry = scene_manager.get_registry();

  app.pause(true);

  auto bird = scene_manager.get_entity("bird");
  registry.get<nl::CShaderProgram>(bird).visible = false;

  auto completed = scene_manager.get_entity("completed");
  nl::CShape& cShape = registry.get<nl::CShape>(completed);
  cShape.size.y = 0.4;
  cShape.meshes.clear();
  cShape.create_mesh();
  registry.get<nl::CTransform>(completed).position.y = 0.3;
  registry.get<nl::CShaderProgram>(completed).visible = true;

  auto restart = scene_manager.get_entity("restart");
  registry.get<nl::CShaderProgram>(restart).visible = true;

  auto menu = scene_manager.get_entity("menu");
  registry.get<nl::CShaderProgram>(menu).visible = true;
}

void CompletedOverlay::on_detach() {
  auto& app = nl::Application::get();
  auto& scene_manager = app.get_scene_manager();
  auto& registry = scene_manager.get_registry();

  auto completed = scene_manager.get_entity("completed");
  registry.get<nl::CShaderProgram>(completed).visible = false;

  auto restart = scene_manager.get_entity("restart");
  registry.get<nl::CShaderProgram>(restart).visible = false;

  auto menu = scene_manager.get_entity("menu");
  registry.get<nl::CShaderProgram>(menu).visible = false;

  app.pause(false);
}

void CompletedOverlay::on_event(nl::Event& e) {
  nl::EventDispatcher dispatcher(e);
  dispatcher.dispatch<nl::KeyPressedEvent>(BIND_EVENT(on_key_pressed));
}

bool CompletedOverlay::on_key_pressed(nl::KeyPressedEvent& e) {
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
    // TODO delete this and import
    if (app.get_states_manager().get_state_index() == 0) {
      app.get_scene_manager().clear_scene();
      app.get_assets_manager().clear();
      app.get_states_manager().push_state(MenuState::create());
    }
    return true;
  } else if (e.get_key() == nl::Key::P) {
    return true;
  }

  return false;
}

std::unique_ptr<nl::Layer> CompletedOverlay::create() {
  return std::make_unique<CompletedOverlay>();
}
}