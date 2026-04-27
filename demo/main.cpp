#include "rendr/context.h"
#include "rendr/window.h"

using namespace rendr;
void rotate_cam(camera& c, float r, float t, const vec3 origin) {
    static float theta = 0;
    theta+= c.speed_*t;
    c.position_.x = r * std::cos(theta) + origin.x;
    c.position_.z = r * std::sin(theta) + origin.z;
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

float compute_delta(float curr_frame) {
    static float last_frame = 0;
    float delta = curr_frame - last_frame;
    last_frame = curr_frame;
    return delta;
}

int main() {
    window win({
        .bg = Black,
        .title = "Import Test"
    });

    rendr::context ctx;

    auto geom = load_obj("assets/robot.obj");
    auto mesh_id = ctx.add_mesh(geom);
    ctx.add_instance(mesh_id);

    auto bb = compute_bbox(geom);
    // auto bbox_mesh = bbox_to_mesh(bb);
    // auto bbox_id = ctx.add_mesh(bbox_mesh);
    // ctx.add_instance(bbox_id);

    float curr_frame{0};
    camera cam;
    cam.target_ = bb.center_;
    cam.position_.y+=500;

    while (win.is_open()) {
        win.poll_event();
        curr_frame = glfwGetTime();

        auto dt = compute_delta(curr_frame);
        compute_fps(curr_frame);

        rotate_cam(cam, 600, dt, bb.center_);
        ctx.update_camera(cam);

        ctx.clear();
        ctx.draw();
        win.display();
    } 
}
