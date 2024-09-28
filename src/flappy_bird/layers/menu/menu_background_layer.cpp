#include "menu_background_layer.h"

#include <entt/entt.hpp>
#include <newleaf/application/application.h>
#include <newleaf/components/graphics/cShaderProgram.h>
#include <newleaf/components/graphics/cTextureAtlas.h>
#include <newleaf/components/physics/cTransform.h>
#include <newleaf/scene/scene_manager.h>

namespace fb {

MenuBackgroundLayer::MenuBackgroundLayer() : Layer("menu_background_layer") {}

void MenuBackgroundLayer::on_attach() {
  auto& app = nl::Application::get();
  auto& scene_manager = app.get_scene_manager();
  auto& registry = scene_manager.get_registry();

  entt::entity bird = scene_manager.get_entity("bird");
  registry.get<nl::CShaderProgram>(bird).visible = true;
  registry.get<nl::CTextureAtlas>(bird).index = 0;
  registry.get<nl::CTransform>(bird).position.y = 0.75;

  entt::entity title = scene_manager.get_entity("title");
  registry.get<nl::CShaderProgram>(title).visible = true;
}

void MenuBackgroundLayer::on_detach() {
  auto& app = nl::Application::get();
  auto& scene_manager = app.get_scene_manager();
  auto& registry = scene_manager.get_registry();

  auto bird = scene_manager.get_entity("bird");
  registry.get<nl::CShaderProgram>(bird).visible = false;

  auto title = scene_manager.get_entity("title");
  registry.get<nl::CShaderProgram>(title).visible = false;
}

std::unique_ptr<nl::Layer> MenuBackgroundLayer::create() {
  return std::make_unique<MenuBackgroundLayer>();
}
}