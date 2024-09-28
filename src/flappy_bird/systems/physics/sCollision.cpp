#include "sCollision.h"

#include <newleaf/application/application.h>
#include <newleaf/components/graphics/cShaderProgram.h>
#include <newleaf/components/meta/cTag.h>
#include <newleaf/components/meta/cUUID.h>
#include <newleaf/components/physics/cCollider.h>
#include <newleaf/components/physics/cTransform.h>
#include <newleaf/events/app_event.h>
#include <newleaf/scene/scene_manager.h>
#include <newleaf/utils/time.h>
#include <newleaf/window/windows_manager.h>

namespace fb {

void CollisionSystem::update(entt::registry& registry, const nl::Time& ts) {
  auto& app = nl::Application::get();
  if (app.is_paused()) {
    return;
  }

  entt::entity bird = app.get_scene_manager().get_entity("bird");
  nl::CTransform& cTransform = registry.get<nl::CTransform>(bird);
  const nl::CCollider& cCollider = registry.get<nl::CCollider>(bird);
  const nl::CUUID& cUUID = registry.get<nl::CUUID>(bird);
  std::string_view collided_with = "";

  for (auto e : registry.view<nl::CTransform, nl::CCollider, nl::CShaderProgram,
                              nl::CTag, nl::CUUID>()) {
    const nl::CTransform& cTransform2 = registry.get<nl::CTransform>(e);
    const nl::CCollider& cCollider2 = registry.get<nl::CCollider>(e);
    const nl::CTag& cTag2 = registry.get<nl::CTag>(e);
    const nl::CUUID& cUUID2 = registry.get<nl::CUUID>(e);
    const nl::CShaderProgram& cSP = registry.get<nl::CShaderProgram>(e);
    if (cUUID.uuid != cUUID2.uuid and cTag2.tag != "buttons") {
      if (cCollider.type == nl::CCollider::Type::rectangle and
          cCollider2.type == nl::CCollider::Type::rectangle) {
        // TODO move logic to class physics
        float minX = cTransform.position.x - cCollider.size.x / 2;
        float maxX = cTransform.position.x + cCollider.size.x / 2;
        float minY = cTransform.position.y - cCollider.size.y / 2;
        float maxY = cTransform.position.y + cCollider.size.y / 2;

        float minX2 = cTransform2.position.x - cCollider2.size.x / 2;
        float maxX2 = cTransform2.position.x + cCollider2.size.x / 2;
        float minY2 = cTransform2.position.y - cCollider2.size.y / 2;
        float maxY2 = cTransform2.position.y + cCollider2.size.y / 2;

        if (minX < maxX2 and maxX > minX2 and minY < maxY2 and maxY > minY2 and
            cSP.visible) {
          collided_with = cTag2.tag;
          if (collided_with == "coin") {
            registry.get<nl::CShaderProgram>(e).visible = false;
          }
          break;
        }
      }
    }
  }

  if (collided_with == "pipe" or collided_with == "ground") {
    app.get_windows_manager().trigger_event(nl::AppUpdateEvent("on_death"));
  } else if (collided_with == "coin") {
    app.get_windows_manager().trigger_event(
      nl::AppUpdateEvent("on_coin_collected"));
  }

  // top off screen check
  if (cTransform.position.y > 0.9) {
    cTransform.position.y = 0.9;
  }
}

}
