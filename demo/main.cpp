#include "glm/ext/matrix_transform.hpp"
#include "rendr/primitives.h"
#include "rendr/system.h"
#include "rendr/camera.h"
#include "rendr/types.h"
#include "rendr/window.h"
#include <print>
#include <random>
#include <vector>

using namespace rendr;
void rotate_cam(camera& c, float r, float t) {
    auto theta  = c.speed_ * t;
    c.position_.x = r * std::cos(theta); 
    c.position_.z = r * std::sin(theta);
}

void spawn_instance(rendr::system& s, std::vector<offset_t>& offs, std::vector<color_t>& cols) {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_real_distribution<float> dist(-600.0f, 600.0f);
    glm::vec4 pos{dist(gen), dist(gen), dist(gen), 1};
    s.add_instance(1, pos);
    offs.push_back(pos);
    cols.push_back(glm::vec4{1});
}

void compute_fps(float curr_frame) {
    static float fps_timer{0}, delta{0}, last_frame{0};
    delta = curr_frame - last_frame;
    last_frame = curr_frame;
    fps_timer += delta;
    if (fps_timer >= 1.f) {
        std::println("fps: {}", 1/delta);
        fps_timer = 0;
    }
}

int main() {
    camera cam;
    cam.speed_/=3;
    window_settings set;
    set.title = "Stress Test: Streaming 1M color and offset instances";

    window win{set};
    rendr::system s;

    float curr_frame{0};
    std::vector<offset_t> offs;
    std::vector<color_t> cols;

    for (int i = 0; i < 1000000; ++i) spawn_instance(s, offs, cols);
    while (win.is_open()) {
        win.poll_event();
        curr_frame = glfwGetTime();
        compute_fps(curr_frame);

        rotate_cam(cam, 2300, curr_frame);

        s.update_offsets(1, offs);
        s.update_colors(1, cols);
        s.update_camera(cam);

        s.clear();
        s.draw();
        win.display();
    }
}
