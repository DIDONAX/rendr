#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "input.h"
#include "rendr/context.h"
#include "rendr/load.h"
#include "rendr/window.h"
#include "utils.h"
#include "editor.h"


using namespace rendr::editor;
int main() {
    rendr::window::settings s = {
        .title_ = "Editor",
        .vsync_ = false,
        .mode_ = rendr::Maximized,
    };

    rendr::window w(s);
    rendr::context ctx;
    rendr::camera cam;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.FontGlobalScale = 1.7f;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(w.instance(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    auto geom = rendr::load_obj("assets/teapot.obj");
    auto bb  = compute_bbox(geom);

    auto mesh_id = ctx.add_mesh(geom);
    ctx.add_instance(mesh_id);

    auto [width, height] = w.frame_buffer_size();
    cam.aspect_ = (float)width/(float)height;
    ctx.set_viewport(0, 0, width, height);

    cam.target_ = bb.center_;
    cam.position_.y += 5.f;

    ctx.update_camera(cam);
    editor_state ed;
    float last_frame = 0.f;

    while (w.is_open()) {
        w.poll_event();

        float now = (float)glfwGetTime();
        ed.delta = now - last_frame;
        ed.fps = ed.delta > 0.f ? 1.f / ed.delta : 0.f;
        last_frame = now;

        handle_input();

        rendr::editor::rotate_cam(cam, 6.f, ed.delta, bb.center_);
        ctx.update_camera(cam);

        ctx.clear();
        ctx.draw();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        tick(ed, ctx);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        w.swap_buffers();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
