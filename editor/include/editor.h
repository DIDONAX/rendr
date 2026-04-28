#pragma once
#include "imgui/imgui.h"
#include "rendr/context.h"
 
namespace rendr::editor {

constexpr auto host_flags = ImGuiWindowFlags_NoTitleBar |
                            ImGuiWindowFlags_NoCollapse |
                            ImGuiWindowFlags_NoResize |
                            ImGuiWindowFlags_NoMove |
                            ImGuiWindowFlags_NoBringToFrontOnFocus |
                            ImGuiWindowFlags_NoNavFocus |
                            ImGuiWindowFlags_MenuBar |
                            ImGuiWindowFlags_NoBackground;

struct editor_state {
    bool show_scene = true;
    bool show_viewport = true;
    bool show_properties = true;
    bool show_console = true;
    int selected_id = -1;
    float fps = 0.f;
    float delta = 0.f;
};
 
// Dockspace
inline void begin_dockspace() {
    const ImGuiViewport* vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(vp->WorkPos);
    ImGui::SetNextWindowSize(vp->WorkSize);
 
 
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("##host", nullptr, host_flags);
    ImGui::PopStyleVar(3);
}
 
inline void end_dockspace() { ImGui::End(); }
 
// Menu bar
inline void draw_menu(editor_state& s) {
    if (!ImGui::BeginMenuBar()) return;
 
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("New Scene")) {}
        if (ImGui::MenuItem("Open...")) {}
        if (ImGui::MenuItem("Save")) {}
        ImGui::Separator();
        if (ImGui::MenuItem("Quit")) {}
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View")) {
        ImGui::MenuItem("Scene", nullptr, &s.show_scene);
        ImGui::MenuItem("Properties", nullptr, &s.show_properties);
        ImGui::EndMenu();
    }
 
    char fps_buf[32];
    snprintf(fps_buf, sizeof(fps_buf), "%.1f fps", s.fps);
    ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - ImGui::CalcTextSize(fps_buf).x - 8.f);
    ImGui::TextDisabled("%s", fps_buf);
 
    ImGui::EndMenuBar();
}

// Panel
inline void draw_scene_panel(editor_state& s, rendr::context&) {
    if (!s.show_scene) return;
    if (!ImGui::Begin("Scene", &s.show_scene)) { ImGui::End(); return; }
 
    ImGui::TextDisabled("Objects");
    ImGui::Separator();
 
    // TODO: iterate ctx objects
    for (int i = 0; i < 4; ++i) {
        auto label = std::format("Object {}", i);
        bool selected = (s.selected_id == i);
        if (ImGui::Selectable(label.c_str(), selected)) {
            s.selected_id = i;
        }
    }
 
    ImGui::End();
}
 
inline void draw_properties_panel(editor_state& s) {
    if (!s.show_properties) return;
    if (!ImGui::Begin("Properties", &s.show_properties)) { ImGui::End(); return; }
 
    if (s.selected_id < 0) {
        ImGui::TextDisabled("Nothing selected");
        ImGui::End();
        return;
    }
 
    ImGui::TextDisabled("Object %d", s.selected_id);
    ImGui::Separator();
 
    // TODO: replace with real component data
    static float pos[3] = {0, 0, 0};
    static float rot[3] = {0, 0, 0};
    static float scl[3] = {1, 1, 1};
    ImGui::DragFloat3("Position", pos, 0.1f);
    ImGui::DragFloat3("Rotation", rot, 0.5f);
    ImGui::DragFloat3("Scale",    scl, 0.05f, 0.001f, 100.f);
 
    ImGui::End();
}
 
// editor tick
 
inline void tick(editor_state& s, rendr::context& ctx) {
    begin_dockspace();
    draw_menu(s);
    end_dockspace();
 
    draw_scene_panel(s, ctx);
    draw_properties_panel(s);
}
 
} // namespace editor
