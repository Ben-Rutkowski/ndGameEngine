#version 330 core

layout (location = 0) in vec4  vPos;
layout (location = 1) in float vSelect;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform sampler2D cull_frame;

out VS_OUT {
    vec2 v;
    float select;
    float w;

    float test_depth;
} vs_out;

void main() {
    vs_out.select = vSelect;
    gl_Position   = proj*view*model*vPos;

    vs_out.w      = gl_Position.w;
    vs_out.v      = (1.0/gl_Position.w)*gl_Position.xy;

    vec4 samp = texture(cull_frame, vs_out.v);
    vs_out.test_depth = samp.x;
}