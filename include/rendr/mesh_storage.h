#pragma once

#include <vector>

#include "rendr/glw.h"
#include "rendr/types.h"
#include "rendr/utils.h"

namespace rendr {

using namespace glw;

struct geometry {
    std::vector<position_t> vertices_;
    std::vector<index_t> indices_;
};

struct mesh_storage_info {
    size_t size_{0};
    size_t capacity_{5};
    size_t vert_capacity_{10000};
    size_t ind_capacity_{10000};
    size_t vert_size_{0};
    size_t ind_size_{0};
};

struct mesh_storage {
    mesh_storage_info info_;

    vertex_array attributes_;
    shader_storage vertices_;
    shader_storage indices_;

    void add(const geometry& geom) {
        auto vert_count = geom.vertices_.size();
        auto ind_count = geom.indices_.size();

        auto& vert_size = info_.vert_size_;
        auto& ind_size = info_.ind_size_;
        assert(vert_count+vert_size <= info_.vert_capacity_ && "vertex capacity reached, resize needed");
        assert(ind_count+ind_count<= info_.ind_capacity_ && "index capacity reached, resize needed");

        host_to_device((position_t*)vertices_.data() + vert_size, geom.vertices_);
        host_to_device((index_t*)indices_.data() + ind_size, geom.indices_);

        info_.size_++;
        vert_size+=vert_count;
        ind_size+=ind_count;
    }
};

} // namespace rendr
