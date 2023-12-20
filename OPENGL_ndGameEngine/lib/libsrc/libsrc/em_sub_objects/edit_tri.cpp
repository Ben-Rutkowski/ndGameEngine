#include "em_sub_objects/edit_tri.hpp"

TriIndexObj::TriIndexObj(Id v0, Id v1, Id v2)
    :vertex_ids{ v0, v1, v2 } {}

Id TriIndexObj::vertId(int i) {
    return vertex_ids[i];
}

// === Calculations ===
vec4 TriIndexObj::calcNorm(VertexCache& vc) {    
    vec4 p0 = vc[vertId(0)].getPos();
    vec4 p1 = vc[vertId(1)].getPos();
    vec4 p2 = vc[vertId(2)].getPos();

    vec4 v1 = vec4::subtrK(p1, p0, 3);
    vec4 v2 = vec4::subtrK(p2, p0, 3);
    vec4 norm = v1.cross(v2);
    norm.normalizeK(3);
    return norm;
}