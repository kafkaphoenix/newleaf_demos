#include "menu_buttons_layer.h"

#include <newleaf/application/application.h>
#include <newleaf/components/graphics/cTextureAtlas.h>
#include <newleaf/components/physics/cTransform.h>
#include <newleaf/scene/scene_manager.h>

#include "handlers/menu/menu_input_handler.h"

namespace fb {

MenuButtonsLayer::MenuButtonsLayer() : Layer("menu_buttons_layer") {}

void MenuButtonsLayer::on_attach() {
  auto& app = nl::Application::get();
  auto& scene_manager = app.get_scene_manager();
  auto& registry = scene_manager.get_registry();

  auto start = scene_manager.create_entity("scene", "buttons", "start");
  registry.get<nl::CTextureAtlas>(start).index = 7;

  auto exit = scene_manager.create_entity("scene", "buttons", "exit");
  registry.get<nl::CTextureAtlas>(exit).index = 6;
  registry.get<nl::CTransform>(exit).position.y = -0.2;

  app.get_render_manager().reorder();
}

void MenuButtonsLayer::on_detach() {
  auto& scene_manager = nl::Application::get().get_scene_manager();

  scene_manager.delete_entity("start");
  scene_manager.delete_entity("exit");
}

void MenuButtonsLayer::on_event(nl::Event& e) { menu_input_handler(e); }

std::unique_ptr<nl::Layer> MenuButtonsLayer::create() { return std::make_unique<MenuButtonsLayer>(); }
}