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

namespace imgui {
    inline void init(const window& w) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.FontGlobalScale = 1.7f;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(w.instance(), true);
        ImGui_ImplOpenGL3_Init("#version 460");
    }

    inline void destroy() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    inline void begin_frame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    inline void end_frame() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}


}
