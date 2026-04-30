#pragma once

#include <rendr/types.h>

namespace rendr {

struct draw_command {
    uint count_;
    uint32_t instance_count;
    uint first_index_;
    int  base_vertex_;
    uint base_instance_; // offset in global per instance data in num el
};

static_assert(sizeof(draw_command) == 20);

} // namespace rendr
