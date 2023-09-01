#version 330 core

in  vec4 fCol;
out vec4 fragCol;

void main() {
    fragCol = fCol;
    // fragCol = vec4(0.0, 1.0, 0.0, 1.0);
}