#pragma once

#include "glad/gl.h"
#include "rendr/types.h"

namespace rendr {
constexpr auto kSegments = 30;
constexpr auto kMaxInstances = 1000;
constexpr auto kNumMeshes = 3;

constexpr auto triangle_id = 0;
constexpr auto rectangle_id = 1;
constexpr auto circle_id = 2;

constexpr auto White = glm::vec4(1.0f);

} // namespace rendr
