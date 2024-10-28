#include "sAnimation.h"

#include <newleaf/application/application.h>
#include <newleaf/components/graphics/cTextureAtlas.h>
#include <newleaf/components/meta/cTag.h>
#include <newleaf/components/meta/cUUID.h>
#include <newleaf/components/physics/cRigidBody.h>
#include <newleaf/components/physics/cTransform.h>

// TODO REMOVE
static uint32_t bird_state{};
static uint32_t coin_state{};
static float bird_delay{};
static float coin_delay{};
static float rotation{};
static bool rotate = true;

namespace fb {

void AnimationSystem::update(entt::registry& registry, const nl::Time& ts) {
  if (nl::Application::get().is_paused()) {
    return;
  }

  registry.view<nl::CTransform, nl::CRigidBody, nl::CTag, nl::CUUID>().each(
    [&](entt::entity e, nl::CTransform& cTransform,
        const nl::CRigidBody& cRigidBody, const nl::CTag& cTag,
        const nl::CUUID& cUUID) {
      if (cRigidBody.kinematic) {
        if (cTag.tag == "bird") {
          nl::CTextureAtlas& cTextureAtlas = registry.get<nl::CTextureAtlas>(e);

          bird_delay += ts;

          if (bird_delay > 0.1) {
            bird_delay = 0;
            cTextureAtlas.index = bird_state++;
            if (bird_state > 2) {
              bird_state = 0;
            }
          }

          if (rotate) {
            rotation += 0.1f;
            if (rotation > 0.5f) {
              rotate = false;
            }
          } else {
            rotation -= 0.1f;
            if (rotation < -0.5f) {
              rotate = true;
            }
          }
          cTransform.rotate(rotation, {0.0f, 0.0f, 1.0f});
        } else if (cTag.tag == "coin") {
          nl::CTextureAtlas& cTextureAtlas = registry.get<nl::CTextureAtlas>(e);

          coin_delay += ts / 4; // TODO should depends on a value speed

          if (coin_delay > 0.1) {
            coin_delay = 0;
            cTextureAtlas.index = coin_state++;
            if (coin_state > 5) {
              coin_state = 0;
            }
          }
        }
      }
    });
}
}
