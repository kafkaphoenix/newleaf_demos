#include "sBirdMovement.h"

#include <glm/glm.hpp>
#include <newleaf/application/application.h>
#include <newleaf/components/input/cActiveInput.h>
#include <newleaf/components/input/cInput.h>
#include <newleaf/components/meta/cUUID.h>
#include <newleaf/components/physics/cRigidBody.h>
#include <newleaf/components/physics/cTransform.h>
#include <newleaf/window/input.h>
#include <newleaf/window/key_codes.h>

namespace fb {

void BirdMovementSystem::update(entt::registry& registry, const nl::Time& ts) {
  auto& app = nl::Application::get();
  if (app.is_debugging() or app.is_paused()) {
    return;
  }

  entt::entity bird = registry.view<nl::CTransform, nl::CRigidBody, nl::CInput, nl::CActiveInput, nl::CUUID>().front();
  APP_ASSERT(bird not_eq entt::null, "no bird found");
  const nl::CRigidBody& cRigidBody = registry.get<nl::CRigidBody>(bird);

  if (cRigidBody.kinematic) {
    const nl::CInput& cInput = registry.get<nl::CInput>(bird);
    APP_ASSERT(cInput.mode == nl::CInput::Mode::_2d, "only 2d input mode supported in this demo");

    if (nl::Input::is_key_pressed(nl::Key::Space)) {
      nl::CTransform& cTransform = registry.get<nl::CTransform>(bird);
      float speed = ts * cInput.translation_speed;
      cTransform.position += glm::vec3(0, 1, 0) * speed;
    }
  }
}
}
