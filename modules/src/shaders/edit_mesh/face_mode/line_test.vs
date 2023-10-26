#version 330 core

layout (location = 0) in vec4  vPos;
layout (location = 1) in float vSelect;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out VS_OUT {
    float select;
    float depth;
} vs_out;

uniform sampler2D cull_frame;

void main() {
    vs_out.select = vSelect;
    gl_Position   = proj*view*model*vPos;

    float w = gl_Position.w;
    vec2  v = (1.0/w)*gl_Position.xy;
    vec2  vc = 0.5*(v + vec2(1,1));

    // float depth = 2.0*texture(cull_frame, v.xy).x - 1.0;
    vs_out.depth = texture(cull_frame, vc.xy).x;
    // vs_out.depth = 1.0;
}