#pragma once

#include <string>

#include "GLFW/glfw3.h"

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
    Space = GLFW_KEY_SPACE,
    LCTRL = GLFW_KEY_LEFT_CONTROL
};

enum Mouse {
    Left = GLFW_MOUSE_BUTTON_LEFT,
    Right = GLFW_MOUSE_BUTTON_RIGHT
};

enum Mode {
    Windowed,
    Maximized,
    Fullscreen
};

using fb_size_t = std::pair<int, int>;
using w_size_t = std::pair<int, int>;

class window {
    public:
        struct settings {
            std::string title_{"Default"};
            int height_{900};
            int width_{900};
            bool vsync_{false};
            Mode mode_{Windowed};
        };

        window();
        window(const settings& s);
        ~window();

        bool is_open() const;
        void swap_buffers() const;
        void poll_event() const;
        fb_size_t frame_buffer_size() const;
        w_size_t size() const;
        GLFWwindow* instance() const;

    private:
        GLFWwindow* glf_window_;
        settings settings_{};
        void init();
        void resolve(settings&) const;
};

} // namespace rendr
