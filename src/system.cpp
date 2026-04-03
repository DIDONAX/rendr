#include "rendr/system.h"
#include "GLFW/glfw3.h"
#include "rendr/window.h"

namespace rendr {

system::system() {
    glfwInit();
    window_ = new glfw::window();
};
system::~system() {
    delete window_;
    glfwTerminate();
};
void system::draw(){};

}  // namespace rendr
