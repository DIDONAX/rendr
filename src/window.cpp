#include "rendr/window.h"
#include "GLFW/glfw3.h"
#include "glad/gl.h"

namespace rendr {


glfw::window::~window() { glfwDestroyWindow(glf_window); }

glfw::window::window() {
    auto create_window = [&]() {
        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
        glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
        auto monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        glf_window = glfwCreateWindow(mode->width, mode->height, "My Title", monitor, NULL);
        glfwSetWindowPos(glf_window, 0, 0);
    };

    create_window();
    glfwMakeContextCurrent(glf_window);
    gladLoaderLoadGL();
    glfwSwapInterval(0);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}



} // namespace rendr
