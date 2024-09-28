#include "menu_buttons_layer.h"

#include <newleaf/application/application.h>
#include <newleaf/components/graphics/cShaderProgram.h>
#include <newleaf/scene/scene_manager.h>

#include "dispatchers/menu/menu_input_dispatcher.h"

namespace fb {

MenuButtonsLayer::MenuButtonsLayer() : Layer("menu_buttons_layer") {}

void MenuButtonsLayer::on_attach() {
  auto& app = nl::Application::get();
  auto& scene_manager = app.get_scene_manager();
  auto& registry = scene_manager.get_registry();

  auto start = scene_manager.get_entity("start");
  registry.get<nl::CShaderProgram>(start).visible = true;

  auto exit = scene_manager.get_entity("exit");
  registry.get<nl::CShaderProgram>(exit).visible = true;
}

void MenuButtonsLayer::on_detach() {
  auto& app = nl::Application::get();
  auto& scene_manager = app.get_scene_manager();
  auto& registry = scene_manager.get_registry();

  auto start = scene_manager.get_entity("start");
  registry.get<nl::CShaderProgram>(start).visible = false;

  auto exit = scene_manager.get_entity("exit");
  registry.get<nl::CShaderProgram>(exit).visible = false;
}

void MenuButtonsLayer::on_event(nl::Event& e) { menu_input_dispatcher(e); }

std::unique_ptr<nl::Layer> MenuButtonsLayer::create() {
  return std::make_unique<MenuButtonsLayer>();
}
}