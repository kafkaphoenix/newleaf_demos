#include "sPlayerMovement.h"

#include <glm/glm.hpp>
#include <newleaf/application/application.h>
#include <newleaf/components/camera/cCamera.h>
#include <newleaf/components/input/cActiveInput.h>
#include <newleaf/components/input/cInput.h>
#include <newleaf/components/meta/cUUID.h>
#include <newleaf/components/physics/cRigidBody.h>
#include <newleaf/components/physics/cTransform.h>
#include <newleaf/window/input.h>
#include <newleaf/window/key_codes.h>

namespace spz {

void PlayerMovementSystem::update(entt::registry& registry, const nl::Time& ts) {
  auto& app = nl::Application::get();
  if (app.is_debugging() or app.is_paused()) {
    return;
  }

  entt::entity player =
    registry.view<nl::CTransform, nl::CRigidBody, nl::CActiveInput, nl::CInput, nl::CCamera, nl::CUUID>().front();
  APP_ASSERT(player not_eq entt::null, "no player found");
  const nl::CRigidBody& cRigidBody = registry.get<nl::CRigidBody>(player);

  if (cRigidBody.kinematic) {
    const nl::CInput& cInput = registry.get<nl::CInput>(player);
    APP_ASSERT(cInput.mode == nl::CInput::Mode::_3d, "only 3d input mode supported in this demo");
    nl::CCamera& cCamera = registry.get<nl::CCamera>(player);
    APP_ASSERT(cCamera.mode == nl::CCamera::Mode::_3d, "only 3d camera mode supported in this demo");

    nl::CTransform& cTransform = registry.get<nl::CTransform>(player);
    float speed = ts * cInput.translation_speed;
    glm::quat qf = cTransform.rotation * glm::quat(0, 0, 0, -1) * glm::conjugate(cTransform.rotation);
    glm::vec3 front = glm::normalize(glm::vec3(qf.x, qf.y, qf.z));
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
    glm::vec3 up = {0, ts * cInput.vertical_speed, 0};

    if (nl::Input::is_key_pressed(nl::Key::W))
      cTransform.position += front * speed;

    if (nl::Input::is_key_pressed(nl::Key::S))
      cTransform.position -= front * speed;

    if (nl::Input::is_key_pressed(nl::Key::A))
      cTransform.position -= right * speed;

    if (nl::Input::is_key_pressed(nl::Key::D))
      cTransform.position += right * speed;

    if (nl::Input::is_key_pressed(nl::Key::Space))
      cTransform.position += up;

    // Warning: with sticky keys on, it will keep falling down after pressing it
    // as we are polling the input state instead of using the event system
    if (nl::Input::is_key_pressed(nl::Key::LeftControl))
      cTransform.position -= up;

    cCamera.calculate_view(cTransform.position, cTransform.rotation);
  }
}
}
