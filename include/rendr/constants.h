#pragma once

#include <cassert>
#include <limits.h>

#include "rendr/types.h"

namespace rendr {

constexpr auto kIndexCapacity = 5000000U;
constexpr auto kVertexCapacity = 5000000U;
constexpr auto kMeshCapacity= 7U;

constexpr auto kInstanceCapacity = 1000000U;
static_assert(kInstanceCapacity < std::numeric_limits<uint>::max());

constexpr color_t White = {1,1,1,1};
constexpr color_t Red = {1,0,0,1};
constexpr color_t Green = {0,1,0,1};
constexpr color_t Blue = {0,0,1,1};
constexpr color_t Black = {0,0,0,1};

}
