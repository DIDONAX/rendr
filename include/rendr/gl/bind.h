#pragma once

#include "glad/gl.h"
#include "rendr/types.h"
#include "rendr/gl/enums.h"

namespace rendr {

template<ObjectType T, const uint L> requires (T == UniformBuffer || T == StorageBuffer)
inline void bind(const uint id) {
    glBindBufferBase(T, L, id);
}

template<ObjectType T> requires (T == VertexLayout)
inline void bind(const uint id) {
    glBindVertexArray(id);
}

template<ObjectType T> requires (T == VertexBuffer || T == IndexBuffer || T == IndirectBuffer)
inline void bind(const uint id) {
    glBindBuffer(T, id);
}

template<ObjectType T> requires (T == TextureBuffer)
inline void bind(const uint id, const GLenum internal_format, const uint buffer_id) {
    glTextureBuffer(id, internal_format, buffer_id);
}

}
