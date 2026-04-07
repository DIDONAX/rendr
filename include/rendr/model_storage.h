#pragma once

#include <vector>

#include "rendr/glw.h"
#include "rendr/types.h"

namespace rendr {
using namespace glw;

struct model_storage {
    // per mesh
    size_t capacity_{1000};
    shader_storage offsets_;
    shader_storage colors_;
};

} // namespace rendr
