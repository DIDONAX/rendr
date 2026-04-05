#pragma once

#include <vector>

#include "rendr/glw.h"
#include "rendr/types.h"

namespace rendr {

struct model_storage {
    // per mesh
    size_t capacity_{300000};
    glw::vertex_buffer offsets_;
};

} // namespace rendr
