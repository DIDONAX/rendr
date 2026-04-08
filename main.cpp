#include "glm/geometric.hpp"
#include "rendr/primitives.h"
#include "rendr/system.h"
#include "rendr/camera.h"
#include "rendr/window.h"
#include <print>
#include <vector>

using namespace rendr;
int main() {
    window_settings settings;
    camera cam;
    cam.speed_ = 10;
    window win(settings);
    rendr::system s;

    float last_frame{0};
    float curr_frame{0};
    int num{0};
    auto process_input = [&](auto dt) {
        auto& pos = cam.position_;
        auto off = cam.speed_ * dt;

        if(win.key(A) == Press) pos.x-= off;
        if(win.key(S) == Press) pos.z+= off;
        if(win.key(D) == Press) pos.x+= off;
        if(win.key(W) == Press) pos.z-= off;

        cam.target_ = pos + glm::vec3{0,0,-1};

        if(win.mouse(Left) == Press) s.add_instance(1, {cam.target_, 1});
    };

    std::vector<offset_t> offs;
    offs.resize(1000000);
    int grid = 1000;
    float step = 2.0f/(grid - 1);

    for (int y = 0; y < grid; y++) {
        for (int x = 0; x < grid; x++) {
            float px = -1.0f + x * step;
            float py = -1.0f + y * step;
            glm::vec4 pos = {px, py, 0.f, 1.f};
            s.add_instance(0, pos);
            offs.push_back(pos);
        }
    }

    float delta{0}; 
    while (win.is_open()) {
        win.poll_event();
        curr_frame = glfwGetTime();
        delta = curr_frame-last_frame;
        last_frame = curr_frame;
        process_input(delta);
        win.clear();
 
        std::println("fps: {}, num: {}", 1000/delta, num);
        s.update_camera(cam);
        s.update_offsets(offs);
        s.draw();
        win.display();
    }
    return 0;
}


