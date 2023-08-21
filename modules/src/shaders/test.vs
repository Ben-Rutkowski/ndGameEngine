#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec2 vTex;

void main () {
    gl_Position = vec4(vPos.xy, 0.0, 1.0);
}