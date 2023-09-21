#version 330 core
in vec3  fNorm;
in vec3  fFragPos;
in vec3  fCenter;
in float fSelect;

out vec4 fragColor;

uniform vec4 camera_pos;
uniform vec4 color;
uniform vec4 select_color;


const vec3 DARK_COLOR  = 0.6*vec3(0.18, 0.2, 0.25);
const vec3 LIGHT_COLOR = 0.8*vec3(0.8, 0.7, 0.6);
const vec3 SELECT      = vec3(0.8, 0.8, 0.8); 

float calcDiffuse() {
    vec3 frag_normal = normalize(fNorm);
    vec3 to_cam      = normalize(camera_pos.xyz - fCenter);
    // return max(dot(frag_normal, to_cam), 0.0);
    return max(abs(dot(frag_normal, to_cam)), 0.0);
}

void main() {
    float diff = calcDiffuse();
    vec3 lighting_color = mix(DARK_COLOR, LIGHT_COLOR, diff);

    vec3 final_color = mix(lighting_color, SELECT, fSelect);

    fragColor = vec4(final_color, 0.55);
}