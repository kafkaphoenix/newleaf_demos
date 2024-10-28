#include "menu_state.h"

#include <newleaf/application/application.h>
#include <newleaf/scene/scene_manager.h>
#include <newleaf/settings/settings_manager.h>
#include <newleaf/state/states_manager.h>
#include <newleaf/systems/graphics/sRender.h>
#include <newleaf/systems/meta/sDelete.h>

#include "dispatchers/meta/input_dispatcher.h"
#include "dispatchers/meta/window_dispatcher.h"
#include "layers/menu/menu_background_layer.h"
#include "layers/menu/menu_buttons_layer.h"

namespace fb {

MenuState::MenuState() : State("menu_state") {
  auto& app = nl::Application::get();
  const auto& settings_manager = app.get_settings_manager();

  // TODO move to loading screen state
  app.get_scene_manager().create_scene(settings_manager.active_scene,
                                       settings_manager.active_scene_path);
}

void MenuState::on_attach() {
  auto& app = nl::Application::get();
  auto& scene_manager = app.get_scene_manager();
  auto& states_manager = app.get_states_manager();

  states_manager.push_layer(MenuBackgroundLayer::create());
  states_manager.push_layer(MenuButtonsLayer::create());

  scene_manager.register_system("render_system",
                                std::make_unique<nl::RenderSystem>(100));
  scene_manager.register_system("delete_system",
                                std::make_unique<nl::DeleteSystem>(-100));

  scene_manager.create_entity("meta", "camera", "scene_camera");
}

void MenuState::on_detach() {
  auto& app = nl::Application::get();
  auto& scene_manager = app.get_scene_manager();

  app.get_states_manager().get_current_state().get_layers_manager().clear();

  // TODO clear or unregister systems?
  scene_manager.clear_systems();
}

void MenuState::on_event(nl::Event& e) {
  auto& registry = nl::Application::get().get_scene_manager().get_registry();
  window_dispatcher(e, registry);
  input_dispatcher(e, registry);

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

std::unique_ptr<nl::State> MenuState::create() {
  return std::make_unique<MenuState>();
}
}