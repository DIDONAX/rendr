#include "rendr/primitives.h"
#include <cmath>

namespace rendr {

geometry load_cube() {
    geometry g;
    g.vertices_ = {
        {-0.5f,-0.5f, 0.5f}, {0.5f,-0.5f,0.5f}, {0.5f,0.5f,0.5f}, {-0.5f,0.5f,0.5f},
        {-0.5f,-0.5f,-0.5f}, {0.5f,-0.5f,-0.5f}, {0.5f,0.5f,-0.5f}, {-0.5f,0.5f,-0.5f}
    };
    g.indices_ = {
        0,1,2,0,2,3, // f
        1,5,6,1,6,2, // r
        5,4,7,5,7,6, // ba
        4,0,3,4,3,7, // l
        3,2,6,3,6,7, // t
        4,5,1,4,1,0  // bot
    };
    return g;
}

geometry load_triangle() {
    return {
        .vertices_ = {
            {0.0f,0.5f,0.0f},
            {-0.5f,-0.5f,0.0f},
            {0.5f,-0.5f,0.0f}
        },
        .indices_ = {0,1,2}
    };
}

geometry load_quad() {
    return {
        .vertices_ = {
            {-0.5f,-0.5f,0.0f},
            {-0.5f,0.5f,0.0f},
            {0.5f,0.5f,0.0f},
            {0.5f,-0.5f,0.0f},
        },
        .indices_ = {0,1,2,0,2,3}
    };
}

} // namespace rendr
