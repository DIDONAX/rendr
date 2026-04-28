#pragma once

#include "glad/gl.h"
#include "rendr/types.h"
#include "rendr/gl/enums.h"

namespace rendr {

template<BufferType T, const uint L> requires (T == Uniform || T == Storage)
inline void bind(const uint id) {
    glBindBufferBase(T, L, id);
}

template<BufferType T> requires (T == Array)
inline void bind(const uint id) {
    glBindVertexArray(id);
}

template<BufferType T> requires (T != Array && T != Texture)
inline void bind(const uint id) {
    glBindBuffer(T, id);
}

template<BufferType T> requires (T == Texture)
inline void bind(const uint id, const GLenum internal_format, const uint buffer_id) {
    glTextureBuffer(id, internal_format, buffer_id);
}

}
