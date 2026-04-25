#pragma once

#include "rendr/glw.h"
#include "rendr/constants.h"

namespace rendr {

using namespace glw;

struct geometry {
    std::vector<position_t> vertices_;
    std::vector<index_t> indices_;
};

struct mesh_storage {
    vertex_array attributes_;
    mapped_buffer<kVertexCapacity, position_t> vertices_{nullptr, Flags::Static};
    mapped_buffer<kIndexCapacity, index_t> indices_{nullptr, Flags::Static};

    void add(const geometry& geom) {
        indices_.insert(geom.indices_);
        vertices_.insert(geom.vertices_);
    }
};

} // namespace rendr
