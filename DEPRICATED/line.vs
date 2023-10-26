#version 330 core
layout (location = 0) in vec4 pos;
layout (location = 1) in float select;

out float fSelect;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    gl_Position = proj*view*model*pos;

    fSelect = select;
}