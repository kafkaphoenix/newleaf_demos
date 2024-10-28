#pragma once

#include <entt/entt.hpp>
#include <newleaf/components/graphics/cTextureAtlas.h>
#include <newleaf/components/meta/cName.h>
#include <newleaf/components/meta/cUUID.h>

#include "components/meta/cScore.h"

namespace fb {

inline void on_coin_collected(entt::registry& registry) {
  entt::entity game_state = registry.view<CScore, nl::CUUID>().front();
  CScore& score = registry.get<CScore>(game_state);
  int new_score = ++score.score;
  registry.view<nl::CTextureAtlas, nl::CName, nl::CUUID>().each(
    [&](nl::CTextureAtlas& cTextureAtlas, const nl::CName& cName,
        const nl::CUUID& cUUID) {
      if (cName.name == "score_unidades") {
        cTextureAtlas.index = new_score % 10;
      } else if (cName.name == "score_decenas") {
        cTextureAtlas.index = (new_score % 100) / 10;
      } else if (cName.name == "score_centenas") {
        cTextureAtlas.index = new_score / 100;
      }
    });
}
}