#pragma once

#include "glad/gl.h"
#include "glm/ext/matrix_float4x4.hpp"

namespace rendr {

using mesh_id = std::size_t;
using instance_id = std::size_t;

using position_t = glm::vec3;
using index_t = uint;

using offset_t = glm::vec4;
using color_t = glm::vec4;
using rotation_t = glm::mat4;
using scale_t = glm::mat4;

using uint = GLuint;

} // namespace rendr
