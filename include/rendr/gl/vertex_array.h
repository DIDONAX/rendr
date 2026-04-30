#pragma once

#include "rendr/types.h"

namespace rendr {

// TODO:refractor 
struct vertex_array {
    struct attribute {
        uint location_{};
        int size_{};
        uint stride_{}; // relative offset 
    };

    uint id_;

    ~vertex_array(); 
    vertex_array();

    void set(const attribute& attr); 
    void set(const auto& element_buff) const; 
    void set(const auto& vbo, const uint bind_loc, const size_t stride) const;

    void enable_attribute(const uint loc) const; 
    void format_attribute(const uint loc, const int size, const uint rel_off) const; 
    void bind_attribute(const uint loc, const uint bind_loc = 0) const; 
};

void vertex_array::set(const auto& element_buff) const {
    glVertexArrayElementBuffer(id_, element_buff.id());
}

void vertex_array::set(const auto& vertex_buff, const uint bind_loc, const size_t stride) const {
    glVertexArrayVertexBuffer(id_, bind_loc, vertex_buff.id(), 0, stride);
}

}
