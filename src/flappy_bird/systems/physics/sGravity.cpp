#include "sGravity.h"

#include <newleaf/application/application.h>
#include <newleaf/components/meta/cUUID.h>
#include <newleaf/components/physics/cGravity.h>
#include <newleaf/components/physics/cRigidBody.h>
#include <newleaf/components/physics/cTransform.h>

namespace fb {

void GravitySystem::update(entt::registry& registry, const nl::Time& ts) {
  if (nl::Application::get().is_paused()) {
    return;
  }

  registry.view<nl::CTransform, nl::CRigidBody, nl::CGravity, nl::CUUID>().each(
    [&](nl::CTransform& cTransform, const nl::CRigidBody& cRigidBody, const nl::CGravity& cGravity,
        const nl::CUUID& cUUID) {
      if (cRigidBody.kinematic) {
        cTransform.position.y -= cGravity.acceleration * ts;
      }
    });
}
}