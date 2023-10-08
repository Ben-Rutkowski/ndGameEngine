#version 330 core

layout (location = 0) in vec4  vPos;
layout (location = 1) in float vSelect;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out VS_OUT {
    vec2 v;
    float select;
    float w;
} vs_out;

void main() {
    vs_out.select = vSelect;
    gl_Position   = proj*view*model*vPos;

    vs_out.w      = gl_Position.w;
    vs_out.v      = (1.0/gl_Position.w)*gl_Position.xy;

    // gl_Position   = (1.0/gl_Position.w)*gl_Position;
}