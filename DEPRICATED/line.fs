#version 330 core
in float fSelect;

out vec4 fragColor;

uniform vec4 color;
uniform vec4 select_color;

void main() {
    // fragColor = color;
    fragColor = mix(color, select_color, fSelect);
}