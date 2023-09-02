#version 330 core

layout (location = 0) in vec4 vPos;

uniform mat4 view;
uniform mat4 proj;
uniform vec4 orthog;
uniform float offsets[20];

void main() {
    vec3 position = vPos.xyz + offsets[gl_InstanceID]*orthog.xyz;
    gl_Position = proj*view*vec4(position, 1.0);
}