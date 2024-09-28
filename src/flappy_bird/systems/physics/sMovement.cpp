#include "sMovement.h"

#include <glm/glm.hpp>
#include <newleaf/application/application.h>
#include <newleaf/components/input/cInput.h>
#include <newleaf/components/meta/cUUID.h>
#include <newleaf/components/physics/cRigidBody.h>
#include <newleaf/components/physics/cTransform.h>
#include <newleaf/window/input.h>
#include <newleaf/window/key_codes.h>

namespace fb {

void MovementSystem::update(entt::registry& registry, const nl::Time& ts) {
  auto& app = nl::Application::get();
  if (app.is_debugging() or app.is_paused()) {
    return;
  }

  registry.view<nl::CTransform, nl::CRigidBody, nl::CInput, nl::CUUID>().each(
    [&](entt::entity e, nl::CTransform& cTransform,
        const nl::CRigidBody& cRigidBody, const nl::CInput& cInput,
        const nl::CUUID& cUUID) {
      if (cRigidBody.kinematic) {
        float speed = ts * cInput.translation_speed;

        if (nl::Input::is_key_pressed(nl::Key::Space)) {
          cTransform.position += glm::vec3(0, 1, 0) * speed;
        }
      }
    });
}
}
