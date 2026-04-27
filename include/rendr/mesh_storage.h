#pragma once

#include "rendr/geometry.h"
#include "rendr/gl/vertex_array.h"
#include "rendr/gl/mmbuffer.h"
#include "rendr/constants.h"


namespace rendr {

struct mesh_storage {
    vertex_array attributes_;
    mmbuffer<position_t, WriteO> vertices_{};
    mmbuffer<index_t, WriteO> indices_{};

    void add(const geometry& geom) {
        indices_.insert(geom.indices_);
        vertices_.insert(geom.vertices_);
    }
};

} // namespace rendr
