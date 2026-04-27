#pragma once

#include <vector>

#include "rendr/types.h"


namespace rendr {

struct bounding_box {
    position_t min_;
    position_t max_;
    position_t center_;
};

struct geometry {
    std::vector<position_t> vertices_;
    std::vector<index_t> indices_;
};

bounding_box compute_bbox(const geometry& g);
geometry bbox_to_mesh(const bounding_box& bb); 

}
