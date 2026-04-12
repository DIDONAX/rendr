#include "glad/gl.h"
#include "rendr/window.h"

namespace rendr {

window::window(const window_settings& settings) {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    glf_window = glfwCreateWindow(settings.height , settings.height, settings.title.data(), nullptr, nullptr);

    glfwMakeContextCurrent(glf_window);
    gladLoaderLoadGL();
    glfwSwapInterval(settings.vsync);
    disable_cursor(settings.disable_cursor);
    glViewport(0, 0, settings.height, settings.height);
    glClearColor(settings.bg.r, settings.bg.g, settings.bg.b, settings.bg.a);
}

window::~window() {glfwDestroyWindow(glf_window);}

bool window::is_open() const {return!glfwWindowShouldClose(glf_window);}

void window::display() const {glfwSwapBuffers(glf_window);}

void window::poll_event() const {glfwPollEvents();}

State window::key(const enum Key& k) const {
    return static_cast<State>(glfwGetKey(glf_window, k));
}

State window::mouse(const Mouse& m) const {
    return static_cast<State>(glfwGetMouseButton(glf_window, m));
}

glm::vec2 window::mouse_delta() {
    double x, y;
    glfwGetCursorPos(glf_window, &x, &y);

    if (mouse_first_) {
        mouse_last_ = {(float)x, (float)y};
        mouse_first_ = false;
        return {0, 0};
    }

    glm::vec2 delta = {
        ((float)x - mouse_last_.x) / settings_.height,  // already square
        ((float)y - mouse_last_.y) / settings_.height
    };
    mouse_last_ = {(float)x, (float)y};
    return delta;
}

void window::get_mouse_pos(double& x, double& y) const {
    glfwGetCursorPos(glf_window, &x, &y);
}

void window::disable_cursor(bool f) {
    glfwSetInputMode(glf_window, GLFW_CURSOR, f ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

} // namespace rendr
