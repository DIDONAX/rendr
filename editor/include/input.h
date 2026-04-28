#pragma once

#include "imgui/imgui.h"
#include <print>

namespace rendr::editor {

inline void handle_input() {
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) return;

    // Mouse delta
    float dx = io.MouseDelta.x;
    float dy = io.MouseDelta.y;

    if (ImGui::IsMouseDown(ImGuiMouseButton_Middle)) {
        std::println("mouse event: middle");
    }

    if (io.MouseWheel != 0.f) {
        std::println("mouse event: scroll");

    }

    if (io.WantCaptureKeyboard) return;

    if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
        std::println("mouse event: middle");
    }
}

} // namespace rendr::editor

