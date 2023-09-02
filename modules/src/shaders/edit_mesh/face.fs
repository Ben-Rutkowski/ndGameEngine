#version 330 core
in vec4 out_color;
in vec3 fNorm;
out vec4 color;

uniform vec4 camera_front;

void main() {
    vec3 to_cam = normalize(-camera_front.xyz);
    vec3 norm   = normalize(fNorm);
    float diff  = max(dot(norm, to_cam), 0.0);
    vec3 final_color = diff*vec3(0.4, 0.4, 0.4) + vec3(0.1, 0.1, 0.1);
    color = vec4(final_color, 1.0);
}