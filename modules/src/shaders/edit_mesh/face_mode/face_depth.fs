#version 330 core

out vec4 fragColor;

void main() {
    float gamma = 25;

    float depth = gl_FragCoord.z;
    depth = 0.5*(depth+1);
    fragColor   = vec4(depth, depth, depth, 0.0);
}