#include "math/select_algs.hpp"

bool sAlg::crossRight(vec2 q, vec4 r, vec4 e) {
    float t;
    float s;

    t = (q[1]-r[1])/(e[1]-r[1]);
    if (t<=0.0f || t>1.0f) { return false; }

    s = (e[0]-r[0])*t-q[0]+r[0];
    if (s>0.0f) { return false; }
    else        { return true; }
}

float sAlg::triRayIntersect(vec4 v1, vec4 v2, vec4 v3, vec4 u, vec4 d) {
    vec3 c = planeRayIntersect(v1, v2, v3, u, d);

    if      ( c[2] <= 0.0f )               { return -1.0f; }
    else if ( c[0] < 0.0f || c[1] < 0.0f ) { return -1.0f; }
    else if ( c[0] + c[1] >= 1.0f )        { return -1.0f; }
    else { return c[2]; }
}

float sAlg::parRayIntersect(vec4 v1, vec4 v2, vec4 v3, vec4 u, vec4 d) {
    vec3 c = planeRayIntersect(v1, v2, v3, u, d);

    if      ( c[2] <= 0.0f )                { return -1.0f; }
    else if ( c[0] < 0.0f || c[1] < 0.0f )  { return -1.0f; }
    else if ( c[0] >= 1.0f || c[1] >= 1.0f) { return -1.0f; }
    else { return c[2]; }
}

vec3 sAlg::planeRayIntersect(vec4 v1, vec4 v2, vec4 v3, vec4 u, vec4 d) {
    // float TOL = 0.001f;

    d.normalizeK(3);

    vec4 basis_0 = vec4::subtrK(v2, v1, 3);
    vec4 basis_1 = vec4::subtrK(v3, v1, 3);

    // float angle_0 = vec4::dotK(d, basis_0, 3);
    // float angle_1 = vec4::dotK(d, basis_1, 3);

    mat4 A({ basis_0, basis_1, d.multK(-1.0f, 3), vec4::basis(3) });
    vec4 b   = vec4::subtrK(u, v1, 3);

    return mat4::cramer3(A, b);
}

vec4 sAlg::modelToClip(vec4 point, mat4 pvm) {
    vec4 out = pvm*point;
    float iw = 1.0f/out[3];
    out.scalar(iw);
    return out;
}