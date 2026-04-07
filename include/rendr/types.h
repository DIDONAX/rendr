#pragma once

#include <cstddef>
#include <sys/types.h>
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"

namespace rendr {

using mesh_id = size_t;
using instance_id = size_t;

using position_t = glm::vec3;
using offset_t = glm::vec4;
using color_t = glm::vec4;
using normal_t = glm::vec3;
using index_t = uint;

} // namespace rendr
