#pragma once

#include "rendr/geometry.h"
#include "rendr/gl/vertex_array.h"
#include "rendr/gl/mmbuffer.h"

namespace rendr {

struct mesh_data {
    std::size_t ioff_{0};
    std::size_t voff_{0};
    std::size_t icount_{0};
    std::size_t vcount_{0};
};

struct mesh_storage {
    mesh_id next_id_{0};
    std::vector<mesh_data> data_;
    mvector<position_t> vertices_;
    mvector<index_t> indices_;

    //TODO: support different layouts/ move this out of mesh storage 
    vertex_array attributes_;

    //TODO: on remove update the offsets of all the shifted elements
    // void remove(const mesh_id id) {
    // }

    mesh_id add(const geometry& geom) {
        data_.push_back({
            .ioff_ = indices_.size(),
            .voff_ = vertices_.size(),
            .icount_ = geom.indices_.size(),
            .vcount_ = geom.vertices_.size(),
        });

        indices_.insert_back(geom.indices_);
        vertices_.insert_back(geom.vertices_);
        return next_id_++;
    }
};

} // namespace rendr
