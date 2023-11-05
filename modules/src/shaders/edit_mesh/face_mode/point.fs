#version 330 core

in float fSelect;
in float test;

uniform vec4 ambient_color;
uniform vec4 select_color;

out vec4 fragColor;

void main() {
    fragColor = mix(ambient_color, select_color, fSelect);
}