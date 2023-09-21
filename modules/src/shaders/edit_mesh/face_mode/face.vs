#version 330 core

layout (location = 0) in vec4  vPos;
layout (location = 1) in vec4  vNorm;
layout (location = 2) in float vSelect;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform vec4 camera_pos;

out vec3 fCenter;
out vec3 fNorm;
out vec3 fCamera_pos;

void main() {
    gl_Position = proj*view*model*vPos;
    fCenter     = (model*vec4(0.0, 0.0, 0.0, 1.0)).xyz;
    fNorm       = vNorm.xyz;
    fCamera_pos = camera_pos.xyz;
}