#version 460 core

layout(location = 0) in vec3 geom_pos;

layout(std430, binding = 0) readonly buffer Offsets {
    vec4 offsets[];
};

layout(std430, binding = 1) readonly buffer Colors {
    vec4 colors[];
};

layout(std430, binding = 2) readonly buffer Quaternions {
    vec4 quats[];
};

layout(std430, binding = 3) readonly buffer Scales {
    vec4 scales[];
};

layout(std140, binding = 4) uniform Camera {
    mat4 view;
    mat4 proj;
};

out vec4 vcolor;
out vec3 fpos;

void main() {
    uint model_id = gl_BaseInstance + uint(gl_InstanceID);
    vcolor = colors[model_id];

    vec4 t = offsets[model_id];
    t.x = -t.x;
    vec4 q = quats[model_id];
    vec4 s = scales[model_id];

    float x = q.x, y = q.y, z = q.z, w = q.w;
    float x2 = x + x, y2 = y + y, z2 = z + z;

    mat4 model;
    model[0] = vec4(s.x * (1.0 - (y * y2 + z * z2)), s.x * (x * y2 + w * z2), s.x * (x * z2 - w * y2), 0.0);
    model[1] = vec4(s.y * (x * y2 - w * z2), s.y * (1.0 - (x * x2 + z * z2)), s.y * (y * z2 + w * x2), 0.0);
    model[2] = vec4(s.z * (x * z2 + w * y2), s.z * (y * z2 - w * x2), s.z * (1.0 - (x * x2 + y * y2)), 0.0);
    model[3] = t;

    fpos = geom_pos + t.xyz;
    gl_Position = proj * view * model * vec4(geom_pos, 1.0);
}
