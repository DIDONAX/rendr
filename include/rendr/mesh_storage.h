#pragma once

#include "rendr/geometry.h"
#include "rendr/gl/vertex_array.h"
#include "rendr/gl/mmbuffer.h"
#include "rendr/constants.h"


namespace rendr {

struct mesh_storage {
    vertex_array attributes_;
    mvector<position_t> vertices_{};
    mvector<index_t> indices_{};

    void add(const geometry& geom) {
        indices_.insert(geom.indices_);
        vertices_.insert(geom.vertices_);
    }
};

} // namespace rendr
