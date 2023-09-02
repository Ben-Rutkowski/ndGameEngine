#version 330 core

layout (location = 0) in vec4 vPos;
layout (location = 1) in vec4 vNorm;

out vec4 out_color;
out vec3 fNorm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    gl_Position = proj*view*model*vPos;
    out_color   = proj*view*model*vPos;
    fNorm       = vNorm.xyz; 
}