#version 330 core

layout (location = 0) in vec4 vPos;
layout (location = 1) in vec4 vCol;

uniform mat4 view;
uniform mat4 proj;

out vec4 fCol;

void main() {
    gl_Position = proj*view*vPos;
    fCol = vCol;
}