#pragma once

#include <rendr/types.h>

namespace rendr {

struct draw_command {
    uint count_;
    uint32_t instance_count;
    uint first_index_; // offset in num el
    int  base_vertex_; // offset in num el
    uint base_instance_; // offset in global per instance data in num el
};

} // namespace rendr
