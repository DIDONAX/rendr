#pragma once

#include <filesystem>
#include <print>
#include <string>

#include "imgui.h"

#include "rendr/context.h"
#include "rendr/load.h"
#include "rendr/types.h"
#include "rendr/window.h"
#include "rendr/bounding_box.h"

#include "editor.h"

namespace rendr::gui {

namespace fs = std::filesystem;

constexpr auto dir = "assets";

struct editor_state {
    instance_id selected_instance;
    mesh_id selected_mesh;
    std::string selected_file;

    std::vector<bounding_box> bbox_;
    std::vector<fs::path> asset_files_;
    std::vector<instance_id> instances_;
    std::vector<mesh_id> meshes_;
};

static editor_state state_;


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

inline void draw_guizmo(context& ctx) {
    static auto& ctxm = ctx.models_;
    auto& q = ctxm.quaternions_[state_.selected_instance];

    ImGui::SetNextWindowSize(ImVec2(300, 300));
    ImGui::Begin("Guizmo");
    ImGui::gizmo3D("##guizmo", q, 200, imguiGizmo::mode3Axes | imguiGizmo::cubeAtOrigin);
    ImGui::End();
}

inline void draw_properties(context& ctx) {
    static auto& ctxm = ctx.models_;

    ImGui::SetNextWindowSize(ImVec2(400, 300));

    if (ImGui::Begin("Properties")) {

        ImGui::Text("Instance: %d", static_cast<int>(state_.selected_instance));
        ImGui::Separator();

        auto& c = ctxm.colors_[state_.selected_instance];
        ImGui::ColorEdit3("color", glm::value_ptr(c));
        ImGui::Separator();

        auto& o = ctxm.offsets_[state_.selected_instance];
        ImGui::DragFloat3("Offset", &o.x, 0.01f);
        ImGui::Separator();

        auto& s = ctxm.scales_[state_.selected_instance];
        ImGui::DragFloat3("Scale", &s.x, 0.001f);
    }

    ImGui::End();


}

inline void fly_to(editor& e) {
    auto& bb = state_.bbox_[state_.selected_instance];
    auto& cam = e.default_camera();

    cam.target_ = bb.center_;
    cam.position_ = bb.center_ + glm::vec3(0, 0, -10.f);
    cam.direction_ = glm::normalize(cam.target_ - cam.position_);
}

inline void draw_fps() {
    ImGuiIO& io = ImGui::GetIO();

    ImGui::SetNextWindowPos(ImVec2(0, 0));

    ImGui::Begin("fps",
        nullptr,
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoSavedSettings);

    ImGui::Text("FPS: %.1f", io.Framerate);

    ImGui::End();
}

inline void draw_scene(editor& e) {
    ImGui::SetNextWindowSize(ImVec2(400, 300));
    if (ImGui::Begin("Scene")) {
        for (const auto& id : state_.instances_) {
            bool is_selected = (state_.selected_instance == id);
            fly_to(e);
            if (ImGui::Selectable(std::to_string(id).c_str(), is_selected)) state_.selected_instance = id;
        }
    }
    ImGui::End();
}

inline void cache_asset_files(const fs::path& dir) {
    for (const auto& entry : fs::directory_iterator(dir)) {
        const auto& p = entry.path();
        if (p.extension() == ".obj") {
            state_.asset_files_.push_back(p);
        }
    }
}

inline void draw_assets(context& ctx, editor& e) {
    ImGui::SetNextWindowSize(ImVec2(400, 300));
    if (ImGui::Begin("Asset Browser")) {
        if (ImGui::Button("Import") && !state_.selected_file.empty()) {
            auto geom = load_obj(state_.selected_file);
            state_.bbox_.push_back(compute_bbox(geom));

            auto id = ctx.create_mesh(geom);
            state_.meshes_.push_back(id);

            id = ctx.create_instance(id);
            state_.instances_.push_back(id);
            fly_to(e);
        }

        ImGui::Separator();

        for (const auto& p : state_.asset_files_) {
            bool is_selected = (state_.selected_file == p.string());
            if (ImGui::Selectable(p.filename().string().c_str(), is_selected)) state_.selected_file = p.string();
        }
    }

    ImGui::End(); 
}

inline void begin_frame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

inline void end_frame() {
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
}
