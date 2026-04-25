#pragma once

#include <cassert>
#include <limits.h>

#include "rendr/types.h"

namespace rendr {

constexpr auto kIndexCapacity = 50;
static_assert(kIndexCapacity < std::numeric_limits<uint>::max());

constexpr auto kVertexCapacity = 15;
static_assert(kIndexCapacity < std::numeric_limits<uint>::max());

constexpr auto kInstanceCapacity = 1000000;
static_assert(kIndexCapacity < std::numeric_limits<uint>::max());

constexpr auto kMeshCapacity= 3;
static_assert(kIndexCapacity < std::numeric_limits<uint>::max());

constexpr color_t White = {1,1,1,1};
constexpr color_t Red = {1,0,0,1};
constexpr color_t Green = {0,1,0,1};
constexpr color_t Blue = {0,0,1,1};
constexpr color_t Black = {0,0,0,1};

}
