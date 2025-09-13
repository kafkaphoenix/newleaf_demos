#pragma once

#include <map>
#include <string>

#include <newleaf/logging/log_manager.h>
#include <newleaf/utils/numeric_comparator.h>

namespace fb {

struct CPipes {
    uint32_t max_pipes{4};
    uint32_t pipes{0};

    CPipes() = default;
    explicit CPipes(uint32_t mp, uint32_t p) : max_pipes(mp), pipes(p) {}

    void print() const { APP_BACKTRACE("\t\tmax_pipes: {0}\n\t\t\t\t\t\tpipes: {1}", max_pipes, pipes); }

    std::map<std::string, std::string, nl::NumericComparator> to_map() const {
      std::map<std::string, std::string, nl::NumericComparator> info;
      info["max_pipes"] = std::to_string(max_pipes);
      info["pipes"] = std::to_string(pipes);

      return info;
    }
};
}