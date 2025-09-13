#pragma once

#include <map>
#include <string>

#include <newleaf/logging/log_manager.h>
#include <newleaf/utils/numeric_comparator.h>

namespace fb {

struct CTimer {
    uint32_t max_time{20};
    uint32_t left{20};

    CTimer() = default;
    explicit CTimer(uint32_t t, uint32_t l) : max_time(t), left(l) {}

    void print() const { APP_BACKTRACE("\t\tmax_time: {0}\n\t\t\t\t\t\tleft: {1}", max_time, left); }

    std::map<std::string, std::string, nl::NumericComparator> to_map() const {
      std::map<std::string, std::string, nl::NumericComparator> info;
      info["max_time"] = std::to_string(max_time);
      info["left"] = std::to_string(left);

      return info;
    }
};
}