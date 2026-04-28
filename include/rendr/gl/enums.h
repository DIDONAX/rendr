#pragma once

#include "glad/gl.h"
#include "rendr/types.h"

namespace rendr {

enum Flags : flag_t {
    Static = 0,
    Dynamic = GL_DYNAMIC_STORAGE_BIT,
    Read = GL_MAP_READ_BIT,
    Write = GL_MAP_WRITE_BIT,
    Persistent = GL_MAP_PERSISTENT_BIT,
    Coherent = GL_MAP_COHERENT_BIT
};

enum Protection : flag_t{
    ReadO = Read,
    WriteO = Write,
    ReadWrite = Write | Read
};

enum ShaderType {
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
    Geometry = GL_GEOMETRY_SHADER,
    Compute = GL_COMPUTE_SHADER
};

enum BufferType {
    Storage = GL_SHADER_STORAGE_BUFFER
};

}
