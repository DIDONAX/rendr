#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "rendr/context.h"
#include "rendr/window.h"
#include "rendr/camera.h"

namespace rendr {

class editor {
    public:
        editor();
        ~editor();

        bool running() const;
        void display() const;
        context* ctx();
        camera& default_camera();

    private:
        window* window_;
        context* ctx_;
        rendr::camera camera_;
};

inline void init_imgui(const window& w) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.FontGlobalScale = 1.7f;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(w.instance(), true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

inline void destroy_imgui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

}
