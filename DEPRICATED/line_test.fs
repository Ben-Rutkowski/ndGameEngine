#version 330 core

in float fSelect;
in float test;

uniform vec4 ambient_color;
uniform vec4 select_color;

out vec4 fragColor;

void main() {
    fragColor = mix(ambient_color, select_color, fSelect);
    // fragColor = vec4(test, 0.0, 0.0, 1.0);
    // fragColor = vec4(test, test, test, 1.0);
}