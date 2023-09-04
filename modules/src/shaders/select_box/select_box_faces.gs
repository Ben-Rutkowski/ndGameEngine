#version 330 core

layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

void main() {
    vec4 root = gl_in[0].gl_Position;
    vec4 end  = gl_in[1].gl_Position;

    gl_Position = vec4(end.x, root.y, 0.0, 1.0);
    EmitVertex();

    gl_Position = root;
    EmitVertex();

    gl_Position = vec4(end.x, end.y, 0.0, 1.0);
    EmitVertex();

    gl_Position = vec4(root.x, end.y, 0.0, 1.0);
    EmitVertex();

    EndPrimitive();
}