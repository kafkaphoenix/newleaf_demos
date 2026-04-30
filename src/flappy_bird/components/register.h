#pragma once
#include <any>

namespace fb {

struct CCoins;
struct CPipes;
struct CScore;
struct CTimer;

inline CCoins& CastCCoins(std::any other) { return *std::any_cast<CCoins*>(other); }

inline CPipes& CastCPipes(std::any other) { return *std::any_cast<CPipes*>(other); }

inline CScore& CastCScore(std::any other) { return *std::any_cast<CScore*>(other); }

inline CTimer& CastCTimer(std::any other) { return *std::any_cast<CTimer*>(other); }

void register_components();
}