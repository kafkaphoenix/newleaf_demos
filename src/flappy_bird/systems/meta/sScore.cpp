#include "sScore.h"

#include <newleaf/application/application.h>
#include <newleaf/components/graphics/cTextureAtlas.h>
#include <newleaf/components/meta/cTag.h>
#include <newleaf/components/meta/cUUID.h>
#include <newleaf/components/physics/cTransform.h>
#include <newleaf/graphics/render_manager.h>
#include <newleaf/scene/scene_manager.h>

#include "components/meta/cScore.h"

namespace fb {

ScoreSystem::~ScoreSystem() {
  auto& registry = nl::Application::get().get_scene_manager().get_registry();
  for (auto e : registry.view<nl::CTag, nl::CUUID>()) {
    if (registry.get<nl::CTag>(e).tag == "score") {
      registry.destroy(e);
    }
  }
}

void ScoreSystem::init(entt::registry& registry) {
  auto& app = nl::Application::get();
  auto& scene_manager = app.get_scene_manager();

  entt::entity unidades =
    scene_manager.create_entity("scene", "numbers", "score_unidades", "score");
  entt::entity decenas =
    scene_manager.create_entity("scene", "numbers", "score_decenas", "score");
  entt::entity centenas =
    scene_manager.create_entity("scene", "numbers", "score_centenas", "score");

  registry.get<nl::CTransform>(unidades).position = {1.6f, 0.8f, 0.f};
  registry.get<nl::CTransform>(decenas).position = {1.45f, 0.8f, 0.f};
  registry.get<nl::CTransform>(centenas).position = {1.3f, 0.8f, 0.f};

  entt::entity game_state = registry.view<CScore, nl::CUUID>().front();
  int score = registry.get<CScore>(game_state).score;

  registry.get<nl::CTextureAtlas>(centenas).index = score / 100;
  registry.get<nl::CTextureAtlas>(decenas).index = (score % 100) / 10;
  registry.get<nl::CTextureAtlas>(unidades).index = score % 10;

  app.get_render_manager().reorder();
}

}