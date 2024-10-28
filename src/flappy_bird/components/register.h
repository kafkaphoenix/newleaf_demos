#pragma once

namespace fb {

struct CCoins;
struct CPipes;
struct CScore;
struct CTimer;

inline CCoins& CastCCoins(void* other) { return *static_cast<CCoins*>(other); }

inline CPipes& CastCPipes(void* other) { return *static_cast<CPipes*>(other); }

inline CScore& CastCScore(void* other) { return *static_cast<CScore*>(other); }

inline CTimer& CastCTimer(void* other) { return *static_cast<CTimer*>(other); }

void register_components();
}