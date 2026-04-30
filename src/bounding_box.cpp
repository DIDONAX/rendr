#include "rendr/bounding_box.h"
#include "rendr/constants.h"

namespace rendr {

bounding_box compute_bbox(const geometry& g) {
    glm::vec3 min(+kInfinity);
    glm::vec3 max(-kInfinity);
    for (const auto& v : g.vertices_) {
        min = glm::min(min, v);
        max = glm::max(max, v);
    }

    return {
        .min_ = min,
        .max_ = max,
        .center_ = (min+max) * 0.5F
    };
}

geometry bbox_to_mesh(const bounding_box& bb) {
    const auto& mn = bb.min_;
    const auto& mx = bb.max_;

    std::vector<glm::vec3> verts = {
        {mn.x, mn.y, mn.z}, // 0
        {mx.x, mn.y, mn.z}, // 1
        {mx.x, mx.y, mn.z}, // 2
        {mn.x, mx.y, mn.z}, // 3
        {mn.x, mn.y, mx.z}, // 4
        {mx.x, mn.y, mx.z}, // 5
        {mx.x, mx.y, mx.z}, // 6
        {mn.x, mx.y, mx.z}, // 7
    };

    std::vector<uint32_t> indices = {
        0,1,2, 2,3,0, // back
        4,5,6, 6,7,4, // front
        0,4,7, 7,3,0, // left
        1,5,6, 6,2,1, // right
        0,1,5, 5,4,0, // bottom
        3,2,6, 6,7,3, // top
    };

    return { .vertices_ = verts, .indices_ = indices};
}

}
