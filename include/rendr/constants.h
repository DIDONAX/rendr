#pragma once

#include <cassert>
#include <limits.h>

#include "rendr/types.h"

namespace rendr {

constexpr auto kGlobalInstanceCapacity= 10000U;
static_assert(kGlobalInstanceCapacity < std::numeric_limits<uint>::max());

constexpr auto kInfinity = std::numeric_limits<float>::infinity();

constexpr color_t White = {1,1,1,1};
constexpr color_t Red = {1,0,0,1};
constexpr color_t Green = {0,1,0,1};
constexpr color_t Blue = {0,0,1,1};
constexpr color_t Black = {0,0,0,1};

}
