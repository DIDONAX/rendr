#include "glm/geometric.hpp"
#include "rendr/context.h"
#include "rendr/mesh_storage.h"
#include "rendr/primitives.h"
#include "rendr/camera.h"
#include "rendr/load.h"
#include "rendr/constants.h"
#include "rendr/window.h"
#include <cstddef>
#include <print>
#include <random>
#include <vector>

using namespace rendr;
void rotate_cam(camera& c, float r, float t, const vec3 origin) {
    auto theta  = c.speed_ * t;
    c.position_.x = r * std::cos(theta) + origin.x;
    c.position_.z = r * std::sin(theta) + origin.z;
}

void spawn_instance(rendr::context& s, std::vector<offset_t>& offs, std::vector<color_t>& cols) {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_real_distribution<float> dist(-600.0f, 600.0f);
    model_matrix mat;
    mat.offset_ = {dist(gen), dist(gen), dist(gen), 1};
    s.add_instance(cube, mat, Red);
    offs.push_back(mat.offset_);
    cols.push_back(Red);
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
    window win({
        .bg = Black,
        .title = "Stress Test: Streaming 1M color and offset instances" 
    });

    rendr::context ctx;

    auto geom = load_obj("../demo/teapot.obj");
    auto mesh_id = ctx.add_mesh(geom);
    ctx.add_instance(mesh_id, {});


    float curr_frame{0};
    camera cam;
    while (win.is_open()) {
        win.poll_event();
        curr_frame = glfwGetTime();
        compute_fps(curr_frame);

        rotate_cam(cam, 8, curr_frame, cam.target_);
        ctx.update_camera(cam);

        ctx.clear();
        ctx.draw();
        win.display();
    }
}
