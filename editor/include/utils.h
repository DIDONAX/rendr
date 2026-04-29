#pragma once

#include <print>

#include "rendr/camera.h"
#include "rendr/context.h"
#include "rendr/window.h"

namespace rendr::editor {

inline void rotate_cam(camera& c, float r, float t, const vec3 origin) {
    static float theta = 0;
    theta+= c.speed_*t;
    c.position_.x = r * std::cos(theta) + origin.x;
    c.position_.z = r * std::sin(theta) + origin.z;
}

}
