#pragma once

#include "glad/gl.h"
#include "GLFW/glfw3.h"

namespace rendr::glfw {

struct window {
    GLFWwindow* glf_window;
    ~window();
    window();

    inline void clear() const { glClear(GL_COLOR_BUFFER_BIT);}
    inline bool is_open() const { return !glfwWindowShouldClose(glf_window); }
    inline void display() const { glfwSwapBuffers(glf_window); }
    inline void poll_event() const { glfwPollEvents(); }
    inline GLFWwindow* handle() const { return glf_window; }
};

} // namespace rendr
