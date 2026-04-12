#pragma once

#include "rendr/glw.h"

namespace rendr {
using namespace glw;

struct model_storage {
    // per mesh
    size_t capacity_{100000};
    shader_storage offsets_;
    shader_storage colors_;
    shader_storage rotations_;
    shader_storage scales_;
};

} // namespace rendr
