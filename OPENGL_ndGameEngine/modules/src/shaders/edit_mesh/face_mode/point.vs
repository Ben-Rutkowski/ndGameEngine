#version 330 core

layout (location = 0) in vec4  vPos;
layout (location = 1) in float vSelect;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out float fSelect;

void main() {
    fSelect      = vSelect;
    gl_Position  = proj*view*model*vPos;
    gl_PointSize = 3.0;
}