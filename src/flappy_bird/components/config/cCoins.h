#pragma once

#include <map>
#include <string>

#include <newleaf/logging/log_manager.h>
#include <newleaf/utils/numeric_comparator.h>

namespace fb {

struct CCoins {
    uint32_t max_coins{4};
    uint32_t coins{};

    CCoins() = default;
    explicit CCoins(uint32_t mp, uint32_t p) : max_coins(mp), coins(p) {}

    void print() const {
      APP_BACKTRACE("\t\tmax_coins: {0}\n\t\t\t\t\t\tcoins: {1}", max_coins,
                    coins);
    }

    std::map<std::string, std::string, nl::NumericComparator> to_map() const {
      std::map<std::string, std::string, nl::NumericComparator> info;
      info["max_coins"] = std::to_string(max_coins);
      info["coins"] = std::to_string(coins);

      return info;
    }
};
}