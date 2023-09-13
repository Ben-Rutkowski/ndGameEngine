// #include "edit_space/edit_tri.hpp"
#include "em_sub_objects/edit_tri.hpp"

TriIndexObj::TriIndexObj(unsigned int v0, unsigned int v1, unsigned int v2)
    :v{ v0, v1, v2 } {}

vec4 TriIndexObj::getPos(int i, VertexCache& vertex_cache)
    { return vertex_cache[v[i]].getPos(); }
vec4 TriIndexObj::getNorm(int i, VertexCache& vertex_cache)
    { return vertex_cache[v[i]].getNorm(); }
vec2 TriIndexObj::getUV(int i, VertexCache& vertex_cache)
    { return vertex_cache[v[i]].getUV(); }
unsigned int TriIndexObj::getVertId(int i) {
    return v[i];
}

vec4 TriIndexObj::calcNorm(VertexCache& vertex_cache) {    
    vec4 p0 = getPos(0, vertex_cache);
    vec4 p1 = getPos(1, vertex_cache);
    vec4 p2 = getPos(2, vertex_cache);
    vec4 v1 = vec4::subtrK(p1, p0, 3);
    vec4 v2 = vec4::subtrK(p2, p0, 3);
    vec4 norm = v1.cross(v2);
    norm.normalizeK(3);
    return norm;
}