#pragma once

#include "rendr/types.h"
#include "rendr/gl/enums.h"

namespace rendr {

inline void bind(const uint id, const uint32_t binding_location) {
    glBindBufferBase(BufferType::Storage, binding_location, id); 
}

}
