#version 330 core

layout (lines) in;
layout (line_strip, max_vertices = 2) out;

in VS_OUT {
    float select;
    float depth;
} gs_in[];

uniform float width;

out float test;
out float fSelect;

void main() {
    gl_Position = gl_in[0].gl_Position;
    fSelect     = gs_in[0].select;
    test = gs_in[0].depth;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    fSelect     = gs_in[1].select;
    test = gs_in[1].depth;
    EmitVertex();

    EndPrimitive();
}