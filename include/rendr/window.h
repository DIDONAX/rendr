#pragma once

#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "glm/ext/vector_float2.hpp"
#include "rendr/types.h"
#include <string_view>

namespace rendr {

enum State {
    Press = GLFW_PRESS,
    Release = GLFW_RELEASE
};

enum Key {
    A = GLFW_KEY_A,
    W = GLFW_KEY_W,
    S = GLFW_KEY_S,
    D = GLFW_KEY_D,
    R = GLFW_KEY_R,
    C = GLFW_KEY_C,
};

enum Mouse {
    Left = GLFW_MOUSE_BUTTON_LEFT,
    Right = GLFW_MOUSE_BUTTON_RIGHT
};

struct window_settings {
    int height{900};
    bool vsync{false};
    bool disable_cursor{false};
    color_t bg{};
    std::string_view title{"Default"};
};

class window {
    public:
        ~window();
        window(const window_settings& settings = {});

        void clear() const;
        bool is_open() const;
        void display() const;
        void poll_event() const;
        State key(const Key& k) const;
        State mouse(const Mouse& m) const;
        void disable_cursor(bool flag);
        void get_mouse_pos(double& x, double& y) const;
        glm::vec2 mouse_delta();

    private:
        GLFWwindow* glf_window;
        glm::vec2 mouse_last_{0, 0};
        bool mouse_first_{true};
        window_settings settings_{};
};

} // namespace rendr
