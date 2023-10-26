#version 330 core

layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;
// layout (line_strip, max_vertices = 2) out;

in VS_OUT {
    vec2  v;
    float select;
    float w;

    float test_depth;
} gs_in[];

uniform float width;

out float fSelect;

out float test_depthfs;

vec4 normal() {
    vec2 dir =normalize(gs_in[1].w*gl_in[0].gl_Position.xy + gs_in[0].w*gl_in[1].gl_Position.xy);
    return vec4(-dir.y, dir.x, 0.0, 0.0);
}

void main() {
    // vec2 dir  = normalize(gl_in[1].gl_Position.xy - gl_in[0].gl_Position.xy);
    // vec4 norm = width*vec4(-dir.y, dir.x, 0.0, 0.0);

    vec2 dir  = normalize(gs_in[1].v - gs_in[0].v);
    vec4 norm = width*vec4(-dir.y, dir.x, 0.0, 0.0);

    // vec4 norm = width*normal();

    // gl_Position = gl_in[0].gl_Position + width*norm;
    gl_Position = gl_in[0].gl_Position + gs_in[0].w*norm;
    fSelect     = gs_in[0].select;
    test_depthfs = gs_in[0].test_depth;
    EmitVertex();

    // gl_Position = gl_in[0].gl_Position + -width*norm;
    gl_Position = gl_in[0].gl_Position + -gs_in[0].w*norm;
    fSelect     = gs_in[0].select;
    test_depthfs = gs_in[0].test_depth;
    EmitVertex();

    // gl_Position = gl_in[1].gl_Position + width*norm;
    gl_Position = gl_in[1].gl_Position + gs_in[1].w*norm;
    fSelect     = gs_in[1].select;
    test_depthfs = gs_in[1].test_depth;
    EmitVertex();

    // gl_Position = gl_in[1].gl_Position + -width*norm;
    gl_Position = gl_in[1].gl_Position + -gs_in[1].w*norm;
    fSelect     = gs_in[1].select;
    test_depthfs = gs_in[1].test_depth;
    EmitVertex();

    EndPrimitive();
}