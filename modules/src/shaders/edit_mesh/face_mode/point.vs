#version 330 core

layout (location = 0) in vec4  vPos;
layout (location = 1) in float vSelect;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform sampler2D cull_texture;

out float fSelect;
out float test;

vec3 screen(vec4 v) {
    vec3 u = (1.0/v.w)*v.xyz;
    return 0.5*(u + vec3(1.0, 1.0, 1.0));
}

void main() {
    fSelect      = vSelect;
    gl_Position  = proj*view*model*vPos;
    gl_PointSize = 3.0;
}