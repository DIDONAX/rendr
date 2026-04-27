#pragma once

#include "rendr/gl/vertex_array.h"
#include "rendr/gl/mmbuffer.h"
#include "rendr/constants.h"

#include <print>

namespace rendr {

struct geometry {
    std::vector<position_t> vertices_;
    std::vector<index_t> indices_;
};

struct mesh_storage {
    vertex_array attributes_;
    mmbuffer<position_t, WriteO> vertices_{};
    mmbuffer<index_t, WriteO> indices_{};

    void add(const geometry& geom) {
        indices_.insert(geom.indices_);
        vertices_.insert(geom.vertices_);
    }

    void print() {
        for (const auto& i : indices_) {
            std::print("{} |", i);
        }
        for (const auto& v : vertices_) {
            std::print("{},{},{} |", v.x, v.y,v.z);
        }
    }
};

} // namespace rendr
