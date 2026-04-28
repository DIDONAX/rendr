#pragma once

#include <print>

#include "rendr/camera.h"

namespace rendr::editor {

inline void rotate_cam(camera& c, float r, float t, const vec3 origin) {
    static float theta = 0;
    theta+= c.speed_*t;
    c.position_.x = r * std::cos(theta) + origin.x;
    c.position_.z = r * std::sin(theta) + origin.z;
}

inline void compute_fps(float curr_frame) {
    static float fps_timer{0}, delta{0}, last_frame{0};
    delta = curr_frame - last_frame;
    last_frame = curr_frame;
    fps_timer += delta;
    if (fps_timer >= 1.f) {
        std::println("fps: {}", 1/delta);
        fps_timer = 0;
    }
}

inline float compute_delta(float curr_frame) {
    static float last_frame = 0;
    float delta = curr_frame - last_frame;
    last_frame = curr_frame;
    return delta;
}

}
