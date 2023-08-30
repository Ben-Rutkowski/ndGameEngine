#version 330 core
layout (location = 0) in vec4 pos;

void main() {
    gl_Position = pos;
    gl_PointSize = 3.0;
}