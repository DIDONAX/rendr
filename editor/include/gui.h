#pragma once

#include <filesystem>
#include <string>

#include "editor.h"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "rendr/context.h"
#include "rendr/geometry.h"
#include "rendr/gl/mmbuffer.h"
#include "rendr/load.h"
#include "rendr/types.h"
#include "rendr/window.h"

namespace rendr::gui {

namespace fs = std::filesystem;

static bool exit = false;

static instance_id selected_instance = 0;
static mesh_id selected_mesh = 0;
static std::string selected_file = "";

static std::vector<bounding_box> bbox_;
static std::vector<fs::path> asset_files_;
static std::vector<instance_id> instances_;
static std::vector<mesh_id> meshes_;

static float color[] = {1,0,0,1};
constexpr auto dir = "assets";

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

inline void draw_properties(context& ctx) {
    auto& o = ctx.models_.offsets_;
    auto& c = ctx.models_.colors_;
    auto& r = ctx.models_.rotations_;
    auto& s = ctx.models_.scales_;

    ImGui::SetNextWindowSize(ImVec2(400, 300));

    if (ImGui::Begin("Properties")) {

        auto& offset = o[selected_instance];
        auto& color  = c[selected_instance];
        auto& rotation = r[selected_instance];
        auto& scale  = s[selected_instance];

        ImGui::Text("Instance: %d", static_cast<int>(selected_instance));
        ImGui::Separator();

        ImGui::DragFloat4("Offset", &offset.x, 0.1f);
        ImGui::DragFloat4("Color", &color.x, 0.01f, 0.0f, 1.0f);

        ImGui::Separator();
        ImGui::Text("Rotation");
        ImGui::DragFloat4("##rot0", &rotation[0][0], 0.01f);
        ImGui::DragFloat4("##rot1", &rotation[1][0], 0.01f);
        ImGui::DragFloat4("##rot2", &rotation[2][0], 0.01f);
        ImGui::DragFloat4("##rot3", &rotation[3][0], 0.01f);

        ImGui::Separator();
        ImGui::Text("Scale");
        ImGui::DragFloat4("##scl0", &scale[0][0], 0.01f);
        ImGui::DragFloat4("##scl1", &scale[1][0], 0.01f);
        ImGui::DragFloat4("##scl2", &scale[2][0], 0.01f);
        ImGui::DragFloat4("##scl3", &scale[3][0], 0.01f); 
    }

    ImGui::End();
}

inline void draw_scene() {
    ImGui::SetNextWindowSize(ImVec2(400, 300));
    if (ImGui::Begin("Scene")) {
        for (const auto& id : instances_) {
            bool is_selected = (selected_instance == id);
            if (ImGui::Selectable(std::to_string(id).c_str(), is_selected)) selected_instance = id;
        }
    }

    ImGui::End();
}

inline void cache_asset_files(const fs::path& dir) {
    for (const auto& entry : fs::directory_iterator(dir)) {
        const auto& p = entry.path();
        if (p.extension() == ".obj") {
            asset_files_.push_back(p);
        }
    }
}

inline void draw_assets(context& ctx, editor& e) {
    ImGui::SetNextWindowSize(ImVec2(400, 300));
    if (ImGui::Begin("Asset Browser")) {
        if (ImGui::Button("Import") && !selected_file.empty()) {
            auto geom = load_obj(selected_file);
            auto bb = compute_bbox(geom);

            auto id = ctx.add_mesh(geom);
            meshes_.push_back(id);

            id = ctx.add_instance(id);
            instances_.push_back(id);

            // fly to target
            e.default_camera().target_ = bb.center_;
            e.default_camera().position_.y = bb.center_.y;
            e.default_camera().position_.z = -6;
        }

        ImGui::Separator();

        for (const auto& p : asset_files_) {
            bool is_selected = (selected_file == p.string());
            if (ImGui::Selectable(p.filename().string().c_str(), is_selected)) selected_file = p.string();
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
