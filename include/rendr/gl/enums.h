#pragma once

#include "glad/gl.h"

namespace rendr {

enum Flags : GLuint {
    Static = 0,
    Dynamic = GL_DYNAMIC_STORAGE_BIT,
    Read = GL_MAP_READ_BIT,
    Write = GL_MAP_WRITE_BIT,
    Persistent = GL_MAP_PERSISTENT_BIT,
    Coherent = GL_MAP_COHERENT_BIT,
};


enum ShaderType {
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
    Geometry = GL_GEOMETRY_SHADER,
    Compute = GL_COMPUTE_SHADER
};

}
