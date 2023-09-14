#include "em_sub_objects/edit_tri.hpp"

TriIndexObj::TriIndexObj(Id v0, Id v1, Id v2)
    :vertex_ids{ v0, v1, v2 } {}

EditVertex& TriIndexObj::vert(int i, VertexCache& vc) {
    return vc[vertex_ids[i]];
}

Id TriIndexObj::vertId(int i) {
    return vertex_ids[i];
}

// === Calculations ===
vec4 TriIndexObj::calcNorm(VertexCache& vc) {    
    vec4 p0 = vert(0, vc).getPos();
    vec4 p1 = vert(1, vc).getPos();
    vec4 p2 = vert(2, vc).getPos();

    vec4 v1 = vec4::subtrK(p1, p0, 3);
    vec4 v2 = vec4::subtrK(p2, p0, 3);
    vec4 norm = v1.cross(v2);
    norm.normalizeK(3);
    return norm;
}