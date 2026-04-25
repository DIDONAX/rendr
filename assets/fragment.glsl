#version 460 core

in vec3 fpos;
in vec4 vcolor;

out vec4 color;

void main() {
    vec3 light_pos = vec3(5.0, 30.0, 0.0);
    vec3 light_color = vec3(1.0, 1.0, 1.0);
    float ambient_strength = 0.4;

    vec3 light_dir = normalize(light_pos - fpos);

    vec3 fdx = dFdx(fpos);
    vec3 fdy = dFdy(fpos);
    vec3 norm = normalize(cross(fdx, fdy));

    float diff = abs(dot(norm, light_dir));

    vec3 diffuse = diff * light_color;
    vec3 ambient = light_color * ambient_strength;

    vec3 result_rgb = (ambient + diffuse) * vcolor.rgb;
    color = vec4(result_rgb, vcolor.a);
}
