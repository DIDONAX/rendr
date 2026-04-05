#pragma once

namespace rendr {

static const char* VS_SRC = R"glsl(
#version 460 core
layout(location = 0) in vec3 geom_pos;

layout(std430, binding = 0) readonly buffer Offsets {
    vec4 offsets[]; // Use vec4 to avoid std430 padding issues with vec3
};

uniform mat4 view;
uniform mat4 proj;

flat out int vmesh_id; 

void main() {
    uint model_id = gl_BaseInstance + uint(gl_InstanceID);
    vec3 offset = offsets[model_id].xyz;

    vmesh_id = gl_DrawID; 
    gl_Position = proj * view * vec4(geom_pos + offset.xyz, 1.0);
}
)glsl";
 
static const char* FS_SRC = R"glsl(
#version 460 core
flat in int vmesh_id;
out vec4 color;

void main() {
    color = vec4(1.0, 1.0, 1.0, 1.0);
}
)glsl";

} // namespace rendr

