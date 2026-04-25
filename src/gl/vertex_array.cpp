#include "rendr/gl/vertex_array.h"

namespace rendr {

vertex_array::~vertex_array() {if (id_ != 0) glDeleteVertexArrays(1, &id_);}

vertex_array::vertex_array() {glCreateVertexArrays(1, &id_);}

void vertex_array::set(const attribute& attr) {
    glEnableVertexArrayAttrib(id_, attr.location_);
    glVertexArrayAttribFormat(id_, attr.location_, attr.size_, GL_FLOAT, GL_FALSE, attr.stride_);
    glVertexArrayAttribBinding(id_, attr.location_, 0);
}

void vertex_array::set_element_buff(const auto& ebo) const {
    glVertexArrayElementBuffer(id_, ebo.id());
}

void vertex_array::set_vertex_buff(const auto& vbo, const uint bind_loc, const size_t stride) const {
    glVertexArrayVertexBuffer(id_, bind_loc, vbo.id(), 0, stride);
}

void vertex_array::enable_attrib(const uint loc) const {
    glEnableVertexArrayAttrib(id_, loc);
}

void vertex_array::format_attrib(const uint loc, const int size, const uint rel_off) const {
    glVertexArrayAttribFormat(id_, loc, size, GL_FLOAT, GL_FALSE, rel_off);
}

void vertex_array::bind_attrib(const uint loc, const uint bind_loc) const {
    glVertexArrayAttribBinding(id_, loc, bind_loc);
}

}
