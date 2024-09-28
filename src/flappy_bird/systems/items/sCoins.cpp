#include "sCoins.h"

#include <newleaf/application/application.h>
#include <newleaf/components/graphics/cShaderProgram.h>
#include <newleaf/components/meta/cTag.h>
#include <newleaf/components/meta/cUUID.h>
#include <newleaf/components/physics/cTransform.h>
#include <newleaf/components/world/cTime.h>

#include "components/config/CCoins.h"

static float delay2 = 70;

namespace fb {

CoinsSystem::~CoinsSystem() {
  auto& registry = nl::Application::get().get_scene_manager().get_registry();
  for (auto e : registry.view<nl::CTag, nl::CUUID>()) {
    if (registry.get<nl::CTag>(e).tag == "coin") {
      registry.destroy(e);
    }
  }
}

void CoinsSystem::init(entt::registry& registry) {
  auto& app = nl::Application::get();
  auto& scene_manager = app.get_scene_manager();
  auto& render_manager = app.get_render_manager();

  entt::entity game_state = registry.view<CCoins, nl::CUUID>().front();
  CCoins& coins_config = registry.get<CCoins>(game_state);

  for (uint32_t i = 0; i < coins_config.max_coins; i++) {
    entt::entity coin_ = scene_manager.create_entity(
      "scene", "coin", "coin_" + std::to_string(i));
    registry.get<nl::CShaderProgram>(coin_).visible = false;
    registry.get<nl::CTransform>(coin_).position.x = 2.f;
  }
  coins_config.coins = coins_config.max_coins;

  render_manager.reorder();
}

void CoinsSystem::update(entt::registry& registry, const nl::Time& ts) {
  if (nl::Application::get().is_paused()) {
    return;
  }

  entt::entity game_state = registry.view<CCoins, nl::CUUID>().front();
  CCoins& coins_config = registry.get<CCoins>(game_state);

  if (coins_config.coins == 0) {
    coins_config.coins = coins_config.max_coins;
  }

  registry.view<nl::CShaderProgram, nl::CTransform, nl::CTag, nl::CUUID>().each(
    [&](nl::CShaderProgram& cShaderProgram, nl::CTransform& cTransform,
        const nl::CTag& cTag, const nl::CUUID& cUUID) {
      if (cTag.tag == "coin") {
        if (cShaderProgram.visible) {
          float speed = 0.005f; // TODO move to component

          // move coin
          cTransform.position.x -= speed;

          // check if coin is out of screen
          if (cTransform.position.x < -2.f) {
            cShaderProgram.visible = false;
          }
        } else {
          auto e = nl::Application::get().get_scene_manager().get_entity(
            "game_state");
          nl::CTime& cTime = registry.get<nl::CTime>(e);
          if (coins_config.coins > 0 and cTime.current_second % 2 == 0 and
              delay2 == 0) {
            // TODO check if there is a pipe already in the same position
            // randomize y position
            // -0.5 highest 0.6 lowest
            cTransform.position.y =
              -0.5f + static_cast<float>(rand()) /
                        (static_cast<float>(RAND_MAX / 1.1f));
            cTransform.position.x = 2.f;
            cShaderProgram.visible = true;
            coins_config.coins--;
            delay2 += 20;
          } else {
            delay2 -= 1;
            if (delay2 < 0) {
              delay2 = 70;
            }
          }
        }
      }
    });
}
}