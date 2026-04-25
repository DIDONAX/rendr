#pragma once

#include "rendr/glw.h"
#include "rendr/constants.h"

namespace rendr {

using namespace glw;

struct model_storage {
    // per mesh
    mapped_buffer<kInstanceCapacity*kMeshCapacity, offset_t> offsets_{};
    mapped_buffer<kInstanceCapacity*kMeshCapacity, color_t> colors_{};
    mapped_buffer<kInstanceCapacity*kMeshCapacity, rotation_t> rotations_{};
    mapped_buffer<kInstanceCapacity*kMeshCapacity, scale_t> scales_{};
};

} // namespace rendr
