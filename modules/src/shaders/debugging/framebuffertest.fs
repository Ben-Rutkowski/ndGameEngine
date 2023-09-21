#version 330 core

in vec2 fTex;

uniform sampler2D frame_texture;

out vec4 fragColor;

void main() {
    fragColor = texture(frame_texture, fTex);
    // fragColor = vec4(1.0, 1.0, 1.0, 1.0);
}