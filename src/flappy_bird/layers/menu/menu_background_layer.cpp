#include "menu_background_layer.h"

#include <entt/entt.hpp>
#include <newleaf/application/application.h>
#include <newleaf/components/graphics/cTexture.h>
#include <newleaf/components/graphics/cTextureAtlas.h>
#include <newleaf/components/physics/cTransform.h>
#include <newleaf/scene/scene_manager.h>

namespace fb {

MenuBackgroundLayer::MenuBackgroundLayer() : Layer("menu_background_layer") {}

void MenuBackgroundLayer::on_attach() {
  auto& app = nl::Application::get();
  auto& scene_manager = app.get_scene_manager();
  auto& registry = scene_manager.get_registry();

  entt::entity bird = scene_manager.create_entity("creatures", "bird", "bird");
  registry.get<nl::CTextureAtlas>(bird).index = 0;
  registry.get<nl::CTransform>(bird).position.y = 0.75;

  entt::entity title = scene_manager.create_entity("scene", "text", "title");
  registry.get<nl::CTransform>(title).scale = {1.0, 1.0, 1.0};
  registry.get<nl::CTexture>(title).reload_textures({"title"});

  entt::entity background_day = scene_manager.create_entity("scene", "background", "background_day");
  registry.get<nl::CTexture>(background_day).reload_textures({"background_day"});

  scene_manager.create_entity("scene", "ground", "ground");

  app.get_render_manager().reorder();
}

void MenuBackgroundLayer::on_detach() {
  auto& scene_manager = nl::Application::get().get_scene_manager();

  scene_manager.delete_entity("bird");
  scene_manager.delete_entity("title");
  scene_manager.delete_entity("background_day");
  scene_manager.delete_entity("ground");
}

std::unique_ptr<nl::Layer> MenuBackgroundLayer::create() { return std::make_unique<MenuBackgroundLayer>(); }
}