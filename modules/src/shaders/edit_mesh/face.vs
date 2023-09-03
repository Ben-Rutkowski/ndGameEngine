#version 330 core

layout (location = 0) in vec4 vPos;
layout (location = 1) in vec4 vNorm;
layout (location = 2) in vec4 vCenter;

out vec3 fNorm;
out vec3 fFragPos;
out vec3 fCenter;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    gl_Position = proj*view*model*vPos;
    fFragPos    = (model*vPos).xyz;
    fCenter     = (model*vCenter).xyz;
    fNorm       = vNorm.xyz; 
}