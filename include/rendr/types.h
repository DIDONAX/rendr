#pragma once

#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include <sys/types.h>
#include "glm/ext/vector_float4.hpp"
#include "glm/ext/vector_float3.hpp"

namespace rendr {

using color_t = glm::vec4;
using transform_t = glm::vec4;
using position_t = glm::vec4;
using index_t = GLuint;

using dim_t = GLfloat;


} // namespace rendr
