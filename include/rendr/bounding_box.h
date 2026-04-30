#pragma once

#include "rendr/geometry.h"
#include "rendr/types.h"

namespace rendr {

struct bounding_box {
    position_t min_;
    position_t max_;
    position_t center_;
};

bounding_box compute_bbox(const geometry& g);
geometry bbox_to_mesh(const bounding_box& bb); 

}

