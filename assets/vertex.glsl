#version 460 core

layout(location = 0) in vec3 geom_pos;

layout(std430, binding = 0) readonly buffer Offsets {
    vec4 offsets[];
};

layout(std430, binding = 1) readonly buffer Colors {
    vec4 colors[];
};

layout(std430, binding = 2) readonly buffer Rotations {
    mat4 rotations[];
};

layout(std430, binding = 3) readonly buffer Scales {
    mat4 scales[];
};

uniform mat4 view;
uniform mat4 proj;

out vec4 vcolor;
out vec3 fpos;

void main() {
    uint model_id = gl_BaseInstance + uint(gl_InstanceID);
    vec3 offset = offsets[model_id].xyz;
    mat4 rot = rotations[model_id];
    mat4 scale = scales[model_id];
    vcolor = colors[model_id];

    fpos = geom_pos + offset.xyz;

    mat4 model = mat4(1.0);
    model[3] = vec4(offset.xyz, 1.0);
    gl_Position = proj * view * model * rot * scale * vec4(geom_pos, 1.0);
}
