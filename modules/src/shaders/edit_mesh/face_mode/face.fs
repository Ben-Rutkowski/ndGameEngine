#version 330 core

in vec3 fCenter;
in vec3 fNorm;
in vec3 fCamera_pos;

uniform vec4 diffuse_color;
uniform vec4 ambient_color;
uniform vec4 select_color;

out vec4 fragColor;

float calcDiff() {
    vec3 to_camera = normalize(fCamera_pos);
    vec3 normal    = normalize(fNorm);
    return max(dot(to_camera, normal), 0.0);
}

void main() {
    float diff = calcDiff();
    fragColor  = mix(ambient_color, diffuse_color, diff);
}