#pragma once
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"

namespace rendr {

using namespace glm;
struct camera {
    vec3 position_{0, 0,-1};
    vec3 target_{0};
    vec3 direction_{0,0,-1};
    float speed_{1};
    float sens_{1};
    float aspect_{1};
};

inline glm::mat4 compute_view(const camera& c) {
    return glm::lookAt(c.position_, c.target_, {0,1,0}); 
}

} // namespace rendr


