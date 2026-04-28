#include "editor.h"
#include "rendr/camera.h"
#include "rendr/context.h"
#include "rendr/window.h"

namespace rendr {

editor::editor() {
    rendr::window::settings s = {
        .title_ = "Editor",
        .vsync_ = false,
        .mode_ = rendr::Maximized
    };

    window_ = new window(s);
    ctx_ = new context();
    init_imgui(*window_);

    auto [width, height] = window_->frame_buffer_size();
    camera_ = {
        .aspect_ = static_cast<float>(width)/static_cast<float>(height)
    };

    ctx_->set_viewport(0, 0, width, height);
    ctx_->update_camera(camera_);
}

editor::~editor() {
    destroy_imgui();
    delete ctx_;
    delete window_;
}

camera& editor::default_camera() {
    return camera_;
}

context* editor::ctx() {return ctx_;}

bool editor::running() const {
    window_->poll_event();
    ctx_->update_camera(camera_);
    return window_->is_open();
}

void editor::display() const {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    window_->swap_buffers();
}

}
