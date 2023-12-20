#version 330 core

layout (location = 0) in vec2 vCSPos; // Clip Space Position

void main() {
    // vec2 CS_pos = 2.0*vSSPos - vec2(1.0, 1.0);
    // gl_Position = vec4(CS_pos, 0.0, 1.0);
    gl_Position = vec4(vCSPos, 0.0, 1.0);
}