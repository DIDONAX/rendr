#pragma once

#include <vector>

#include "rendr/glw.h"
#include "rendr/types.h"

namespace rendr {

using namespace glw;

struct geometry {
    std::vector<position_t> vertices_;
    std::vector<index_t> indices_;
};

struct mesh_storage_info {
    size_t size_{0};
    size_t capacity_{2};
    size_t vert_capacity_{100};
    size_t ind_capacity_{100};
};

struct mesh_storage {
    mesh_storage_info info_;
    geometry geom_;
    vertex_array attributes_;
    vertex_buffer vertices_;
    vertex_buffer indices_;

    void add(const geometry& geom) {
        geom_.indices_.insert(geom_.indices_.end(), geom.indices_.begin(), geom.indices_.end());
        geom_.vertices_.insert(geom_.vertices_.end(), geom.vertices_.begin(), geom.vertices_.end());
        info_.size_++;
    }
};

} // namespace rendr
