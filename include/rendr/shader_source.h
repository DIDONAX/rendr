#pragma once

namespace rendr {

static const char* vertex_shader = R"glsl(
#version 460 core

layout(location = 0) in vec3 geom_pos;

layout(std430, binding = 0) readonly buffer Offsets {
    vec4 offsets[];
};

layout(std430, binding = 1) readonly buffer Colors {
    vec4 colors[];
};

uniform mat4 view;
uniform mat4 proj;

out vec4 vcolor;
out vec3 fpos;

void main() {
    uint model_id = gl_BaseInstance + uint(gl_InstanceID);
    vec3 offset = offsets[model_id].xyz;
    vcolor = colors[model_id];
    fpos = geom_pos + offset.xyz;

    gl_Position = proj * view * vec4(fpos, 1.0);
}
)glsl";

static const char* fragment_shader = R"glsl(
#version 460 core

in vec3 fpos;

in vec4 vcolor;

out vec4 color;

void main() {
    vec3 light_pos = vec3(0.0, 30.0, 0.0);
    vec3 light_color = vec3(1.0, 1.0, 1.0);
    float ambient_strength = 0.3;

    vec3 light_dir = normalize(light_pos-fpos);

    vec3 fdx = dFdx(fpos);
    vec3 fdy = dFdy(fpos);
    vec3 norm = normalize(cross(fdx, fdy));

    float diff = max(dot(norm, light_dir), 0.0);

    vec3 diffuse = diff * light_color;
    vec3 ambient = light_color * ambient_strength;

    vec3 result_rgb = (ambient + diffuse) * vcolor.rgb;
    color = vec4(result_rgb, vcolor.a);
}
)glsl";

} // namespace rendr

