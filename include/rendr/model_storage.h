#pragma once

#include "rendr/constants.h"
#include "rendr/gl/mapped_buffer.h"

namespace rendr {

struct model_storage {
    // per mesh
    mapped_buffer<kInstanceCapacity*kMeshCapacity, offset_t> offsets_{};
    mapped_buffer<kInstanceCapacity*kMeshCapacity, color_t> colors_{};
    mapped_buffer<kInstanceCapacity*kMeshCapacity, rotation_t> rotations_{};
    mapped_buffer<kInstanceCapacity*kMeshCapacity, scale_t> scales_{};
};

} // namespace rendr
