#include "register.h"

#include <entt/entt.hpp>
#include <newleaf/components/meta.h>

#include "config/cCoins.h"
#include "config/cPipes.h"
#include "meta/cScore.h"
#include "meta/cTimer.h"

using namespace entt::literals;

namespace fb {

void register_components() {
  entt::meta<CCoins>()
    .type("coins"_hs)
    .ctor<&CastCCoins, entt::as_ref_t>()
    .data<&CCoins::max_coins>("max_coins"_hs)
    .data<&CCoins::coins>("coins"_hs)
    .func<&CCoins::print>("print"_hs)
    .func<&CCoins::to_map>("to_map"_hs)
    .func<&nl::assign<CCoins>, entt::as_ref_t>("assign"_hs);

  entt::meta<CPipes>()
    .type("pipes"_hs)
    .ctor<&CastCPipes, entt::as_ref_t>()
    .data<&CPipes::max_pipes>("max_pipes"_hs)
    .data<&CPipes::pipes>("pipes"_hs)
    .func<&CPipes::print>("print"_hs)
    .func<&CPipes::to_map>("to_map"_hs)
    .func<&nl::assign<CPipes>, entt::as_ref_t>("assign"_hs);

  entt::meta<CScore>()
    .type("score"_hs)
    .ctor<&CastCScore, entt::as_ref_t>()
    .data<&CScore::score>("score"_hs)
    .func<&CScore::print>("print"_hs)
    .func<&CScore::to_map>("to_map"_hs)
    .func<&nl::assign<CScore>, entt::as_ref_t>("assign"_hs);

  entt::meta<CTimer>()
    .type("timer"_hs)
    .ctor<&CastCTimer, entt::as_ref_t>()
    .data<&CTimer::max_time>("max_time"_hs)
    .data<&CTimer::left>("left"_hs)
    .func<&CTimer::print>("print"_hs)
    .func<&CTimer::to_map>("to_map"_hs)
    .func<&nl::assign<CTimer>, entt::as_ref_t>("assign"_hs);
}
}