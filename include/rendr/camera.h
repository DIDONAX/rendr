#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
namespace rendr {

struct camera_settings {
    float speed_{2};
    float sens_{0.009};
    float fov_{45};
};

struct camera {
    camera_settings settings_{};
    vec3 pos_ = {0,0,3};
    vec3 front_ = {0,0,-1};
    vec3 up_ = {0,1,0};
    float yaw_{-90};
    float pitch_{0};

    mat4 view() const {
        return lookAt(pos_, pos_ + front_, up_);
    }

    mat4 proj(float aspect, float near = 0.1f, float far = 1000.f) const {
        return perspective(radians(settings_.fov_), aspect, near, far);
    }

    void move(const vec3 dir, const float dt) {
        const auto s = settings_.speed_;
        pos_ += front_ * dir.z * s * dt;
        pos_ += normalize(cross(front_, up_)) * dir.x * s * dt;
        pos_ += up_  * dir.y * s * dt;
    }

    void rotate(const float dx, const float dy) {
        const auto s = settings_.sens_;
        yaw_ += dx * s;
        pitch_ = clamp(pitch_ - dy * s, -89.f, 89.f);
        front_ = normalize(vec3{
            cos(radians(yaw_)) * cos(radians(pitch_)),
            sin(radians(pitch_)),
            sin(radians(yaw_)) * cos(radians(pitch_))
        });
    }
};

} // namespace rendr


