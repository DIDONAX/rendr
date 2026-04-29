#pragma once

#include "glad/gl.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/quaternion.hpp"

namespace rendr {

using mesh_id = std::size_t;
using instance_id = std::size_t;

using position_t = glm::vec3;
using index_t = u_int32_t;

using offset_t = glm::vec4;
using color_t = glm::vec4;
using scale_t = glm::vec4;

// wzyz
using quaternion_t = glm::quat;


using flag_t = GLuint;
using uint = GLuint;

} // namespace rendr
