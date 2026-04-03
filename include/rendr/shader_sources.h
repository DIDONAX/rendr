#pragma once
#include <string_view>

namespace rendr {

constexpr auto vertex_shader_src = R"(
#version 460 core
#extension GL_ARB_shader_draw_parameters : require

layout(std430, binding = 0) readonly buffer Positions {
    vec4 positions[];
};

layout(std430, binding = 1) readonly buffer Transforms {
    vec4 transforms[];
};

layout(std430, binding = 2) readonly buffer Colors {
    vec4 colors[];
};

out vec4 v_color;

void main() {
    int idx     = gl_InstanceID + gl_BaseInstance;
    vec4 pos    = positions[gl_VertexID];
    vec4 model  = transforms[idx];
    v_color     = colors[idx];
    gl_Position = model + pos;
}
)";

constexpr auto fragment_shader_src = R"(
#version 460 core
in  vec4 frag_color;
out vec4 v_color;
void main() {
    frag_color = v_color;
}
)";

} // namespace rendr
