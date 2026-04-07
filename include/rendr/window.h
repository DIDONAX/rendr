#pragma once

#include "glad/gl.h"
#include "GLFW/glfw3.h"

namespace rendr::glfw {

struct window {
    GLFWwindow* glf_window;
    ~window();
    window();

    inline void clear() const {glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);}
    inline bool is_open() const {return !glfwWindowShouldClose(glf_window);}
    inline void display() const {glfwSwapBuffers(glf_window);}
    inline void poll_event() const {glfwPollEvents();}
    inline GLFWwindow* handle() const {return glf_window;}
    inline void set_cursor_pos_cb(GLFWcursorposfun cb) const {glfwSetCursorPosCallback(glf_window, cb);}
    inline void set_key_cb(GLFWkeyfun cb) const {glfwSetKeyCallback(glf_window, cb); }
    inline void set_scroll_cb(GLFWscrollfun cb) const {glfwSetScrollCallback(glf_window, cb);}
    inline float aspect() const {
        int w, h;
        glfwGetFramebufferSize(glf_window, &w, &h);
        return static_cast<float>(w) / h;
    }
    inline void lock_cursor() const {glfwSetInputMode(glf_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);}
    inline void unlock_cursor() const {glfwSetInputMode(glf_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);}
    inline bool key(int glfwKey) const { return glfwGetKey(glf_window, glfwKey) == GLFW_PRESS; }
};

} // namespace rendr
