#include "editor.h"
#include "rendr/camera.h"
#include "rendr/context.h"
#include "rendr/window.h"

// example usage of the rendr library, OpenGl context lifecycle and windowing all managed application level, the library operates on the current bound context if provided

namespace rendr {

editor::editor() {
    rendr::window::settings s = {
        .title_ = "Editor",
        .vsync_ = false,
        .mode_ = rendr::Maximized
    };

    window_ = new window(s);
    ctx_ = new context();
    imgui::init(*window_);

    auto [width, height] = window_->frame_buffer_size();
    camera_ = {
        .aspect_ = static_cast<float>(width)/static_cast<float>(height)
    };

    ctx_->set_viewport(0, 0, width, height);
    ctx_->update_camera(camera_);
}

editor::~editor() {
    imgui::destroy();
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
    ctx_->clear();
    ctx_->draw();

    imgui::begin_frame();
    //
    imgui::end_frame();

    window_->swap_buffers();
}

}
