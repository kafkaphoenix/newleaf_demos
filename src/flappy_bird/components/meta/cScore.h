#pragma once

#include <map>
#include <string>

#include <newleaf/logging/log_manager.h>
#include <newleaf/utils/numeric_comparator.h>

namespace fb {

struct CScore {
    int score{};

    CScore() = default;
    explicit CScore(int s) : score(s) {}

    void print() const { APP_BACKTRACE("\t\tscore: {0}", score); }

    std::map<std::string, std::string, nl::NumericComparator> to_map() const {
      std::map<std::string, std::string, nl::NumericComparator> info;
      info["score"] = std::to_string(score);

      return info;
    }
};
}