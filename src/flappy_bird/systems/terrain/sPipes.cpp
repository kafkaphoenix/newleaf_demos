#include "sPipes.h"

#include <newleaf/application/application.h>
#include <newleaf/components/graphics/cShaderProgram.h>
#include <newleaf/components/graphics/cTexture.h>
#include <newleaf/components/meta/cName.h>
#include <newleaf/components/meta/cTag.h>
#include <newleaf/components/meta/cUUID.h>
#include <newleaf/components/physics/cTransform.h>
#include <newleaf/components/world/cTime.h>
#include <newleaf/graphics/render_manager.h>
#include <newleaf/scene/scene_manager.h>

#include "components/config/cPipes.h"

static float delay = 50;

namespace fb {

PipesSystem::~PipesSystem() {
  auto& registry = nl::Application::get().get_scene_manager().get_registry();
  for (auto e : registry.view<nl::CTag, nl::CUUID>()) {
    if (registry.get<nl::CTag>(e).tag == "pipe") {
      registry.destroy(e);
    }
  }
}

void PipesSystem::init(entt::registry& registry) {
  auto& app = nl::Application::get();
  auto& scene_manager = app.get_scene_manager();

  entt::entity game_state = registry.view<CPipes, nl::CUUID>().front();
  CPipes& pipes_config = registry.get<CPipes>(game_state);

  entt::entity green_pipe_top =
    scene_manager.create_entity("scene", "pipe", "green_pipe_top");
  registry.get<nl::CTexture>(green_pipe_top)
    .reload_textures({
      "green_pipe_top",
    });
  registry.get<nl::CShaderProgram>(green_pipe_top).visible = false;
  registry.get<nl::CTransform>(green_pipe_top).position.x = 2.f;
  entt::entity green_pipe_bottom =
    scene_manager.create_entity("scene", "pipe", "green_pipe_bottom");
  registry.get<nl::CTexture>(green_pipe_bottom)
    .reload_textures({
      "green_pipe_bottom",
    });
  registry.get<nl::CShaderProgram>(green_pipe_bottom).visible = false;
  registry.get<nl::CTransform>(green_pipe_bottom).position.x = 2.f;
  entt::entity red_pipe_top =
    scene_manager.create_entity("scene", "pipe", "red_pipe_top");
  registry.get<nl::CTexture>(red_pipe_top)
    .reload_textures({
      "red_pipe_top",
    });
  registry.get<nl::CShaderProgram>(red_pipe_top).visible = false;
  registry.get<nl::CTransform>(red_pipe_top).position.x = 2.f;
  entt::entity red_pipe_bottom =
    scene_manager.create_entity("scene", "pipe", "red_pipe_bottom");
  registry.get<nl::CTexture>(red_pipe_bottom)
    .reload_textures({
      "red_pipe_bottom",
    });
  registry.get<nl::CShaderProgram>(red_pipe_bottom).visible = false;
  registry.get<nl::CTransform>(red_pipe_bottom).position.x = 2.f;
  pipes_config.pipes = pipes_config.max_pipes;

  app.get_render_manager().reorder();
}

void PipesSystem::update(entt::registry& registry, const nl::Time& ts) {
  auto& app = nl::Application::get();
  if (app.is_paused()) {
    return;
  }

  entt::entity game_state = registry.view<CPipes, nl::CUUID>().front();
  CPipes& pipes_config = registry.get<CPipes>(game_state);

  if (pipes_config.pipes == 0) {
    pipes_config.pipes = pipes_config.max_pipes;
  }

  registry
    .view<nl::CShaderProgram, nl::CTransform, nl::CTag, nl::CName, nl::CUUID>()
    .each([&](nl::CShaderProgram& cShaderProgram, nl::CTransform& cTransform,
              const nl::CTag& cTag, const nl::CName& cName,
              const nl::CUUID& cUUID) {
      if (cTag.tag == "pipe") {
        if (cShaderProgram.visible) {
          float speed = 0.005f; // TODO move to component

          // move pipe
          cTransform.position.x -= speed;

          // check if pipe is out of screen
          if (cTransform.position.x < -2.f) {
            cShaderProgram.visible = false;
          }
        } else {
          auto e = app.get_scene_manager().get_entity("game_state");
          nl::CTime& cTime = registry.get<nl::CTime>(e);
          if (pipes_config.pipes > 0 and cTime.current_second % 3 == 0 and
              delay == 0) {
            // randomize y position
            if (cName.name.ends_with("top")) { //
              // -0.6 shortest -0.1 longest
              cTransform.position.y =
                -0.6f + static_cast<float>(rand()) /
                          (static_cast<float>(RAND_MAX / 0.5f));
            } else if (cName.name.ends_with("bottom")) {
              // 1.2 shortest 0.7 longest
              cTransform.position.y =
                1.2f - static_cast<float>(rand()) /
                         (static_cast<float>(RAND_MAX / 0.5f));
            }
            cTransform.position.x = 2.f;
            cShaderProgram.visible = true;
            pipes_config.pipes--;
            delay += 30;
          } else {
            delay -= 1;
            if (delay < 0) {
              delay = 50;
            }
          }
        }
      }
    });
}
}