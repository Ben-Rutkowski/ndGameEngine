#include "select_algs.hpp"

bool sAlg::crossRight(vec2 q, vec4 r, vec4 e) {
    float t;
    float s;

    t = (q[1]-r[1])/(e[1]-r[1]);
    if (t<=0.0f || t>1.0f) { return false; }

    s = (e[0]-r[0])*t-q[0]+r[0];
    if (s>0.0f) { return false; }
    else        { return true; }
}

vec4 sAlg::modelToClip(vec4 point, mat4 pvm) {
    vec4 out = pvm*point;
    float iw = 1.0f/out[3];
    out.scalar(iw);
    return out;
}