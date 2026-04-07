#pragma once

#include <vector>

#include "rendr/glw.h"
#include "rendr/types.h"

namespace rendr {

using namespace glw;

// this shoudl be removed 
struct geometry {
    std::vector<position_t> vertices_;
    std::vector<index_t> indices_;
    // std::vector<normal_t> normals_;
};

struct mesh_storage_info {
    size_t size_{0};
    size_t capacity_{5};
    size_t vert_capacity_{1000};
    size_t ind_capacity_{1000};
    // size_t nor_capacity_{1000};
};

struct mesh_storage {
    mesh_storage_info info_;

    // TODO: remove geom: not needed after mapping
    geometry geom_;
    vertex_array attributes_;
    shader_storage vertices_;
    shader_storage indices_;

    void add(const geometry& geom) {
        geom_.indices_.insert(geom_.indices_.end(), geom.indices_.begin(), geom.indices_.end());
        geom_.vertices_.insert(geom_.vertices_.end(), geom.vertices_.begin(), geom.vertices_.end());
        info_.size_++;
    } 
};

} // namespace rendr
