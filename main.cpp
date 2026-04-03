#include "rendr/system.h"
#include "rendr/types.h"
#include <print>
#include <random>
#include <vector>

auto lastTime = glfwGetTime();
auto frameCount = 0;
auto fps = 0;

static std::random_device rd;
static std::mt19937 gen(rd());
void shuffle(std::vector<int>& vec) {
    std::shuffle(vec.begin(), vec.end(), gen);
}

void print_fps() {
    frameCount++;
    double currentTime = glfwGetTime();
    if (currentTime - lastTime >= 1.0) {
        fps = frameCount;
        frameCount = 0;
        lastTime = currentTime;
        std::println("FPS: {}", fps);
    }
};

using namespace rendr;
int main() {
    rendr::system sys;
    std::vector<int> transforms(100000);

    geometry triangle = {
        .positions_ = {
            { 0.0f,  0.5f, 0.0f, 1.0f},
            {-0.5f, -0.5f, 0.0f, 1.0f},
            { 0.5f, -0.5f, 0.0f, 1.0f},
        },
        .indices_ = { 0, 1, 2 }
    };


    while (sys.window_->is_open()) {
        sys.window_->poll_event();
        sys.window_->clear();

        shuffle(transforms);

        sys.draw();

        sys.window_->display();
        print_fps();
    }

    return 0;
}

