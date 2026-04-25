#pragma once

#include "rendr/types.h"

namespace rendr {

constexpr auto kIndexCapacity = 50;
constexpr auto kVertexCapacity = 15;
constexpr auto kInstanceCapacity = 1000000;
constexpr auto kMeshCapacity= 3;

constexpr color_t White = {1,1,1,1};
constexpr color_t Red = {1,0,0,1};
constexpr color_t Green = {0,1,0,1};
constexpr color_t Blue = {0,0,1,1};
constexpr color_t Black = {0,0,0,1};

}
