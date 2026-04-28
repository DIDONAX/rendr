#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "rendr/window.h"
#include <cassert>

namespace rendr {

window::window(const settings& s) : settings_(s) {
    init();
}

window::window() {
    init();
}

window::~window() {glfwDestroyWindow(glf_window_);}

bool window::is_open() const {return !glfwWindowShouldClose(glf_window_);}

void window::swap_buffers() const {glfwSwapBuffers(glf_window_);}

void window::poll_event() const {glfwPollEvents();}

w_size_t window::size() const {
    return w_size_t(settings_.width_, settings_.height_);
}

fb_size_t window::frame_buffer_size() const {
    fb_size_t size;
    glfwGetFramebufferSize(glf_window_, &size.first, &size.second);
    return size;
}

GLFWwindow* window::instance() const { return glf_window_;}

void window::init() {
    assert(glfwInit());

    GLFWmonitor* monitor = settings_.mode_ == Fullscreen ? glfwGetPrimaryMonitor() : nullptr;

    if (settings_.mode_ == Maximized) {
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    glf_window_ = glfwCreateWindow(
        settings_.width_,
        settings_.height_,
        settings_.title_.c_str(),
        monitor,
        nullptr
    );
    assert(glf_window_);

    glfwMakeContextCurrent(glf_window_);
    assert(gladLoaderLoadGL());

    glfwSwapInterval(settings_.vsync_);

    auto [w, h] = this->size();
    settings_.width_ = w;
    settings_.height_ = h;
}

} // namespace rendr
