#pragma once

#include "rendr/draw_command.h"
#include "rendr/gl/enums.h"
#include <cstddef>

namespace rendr {

enum Primitive {
    Triangle = GL_TRIANGLES,
    Line = GL_LINES,
    Point = GL_POINTS,
    TriangleStrip = GL_TRIANGLE_STRIP,
    TriangleFan = GL_TRIANGLE_FAN,
    LineStrip = GL_LINE_STRIP,
    LineLoop = GL_LINE_LOOP,
};

template<Primitive P>
inline void multi_draw(const std::size_t offset, const std::size_t size) {
    glMultiDrawElementsIndirect(P, GL_UNSIGNED_INT, reinterpret_cast<void*>(offset * sizeof(draw_command)), size, 0);
}
static_assert(std::is_same_v<index_t, uint32_t>);

}
