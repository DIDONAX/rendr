#pragma once

#include <vector>

#include "rendr/types.h"


namespace rendr {

struct geometry {
    std::vector<position_t> vertices_;
    std::vector<index_t> indices_;
};

}
