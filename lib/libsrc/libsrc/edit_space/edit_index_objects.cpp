#include "edit_space/edit_index_objects.hpp"

// ======== EdgeIndexObj ========
EdgeIndexObj::EdgeIndexObj(unsigned int tail, unsigned int tip)
    :p{ tail, tip } {}

vec4 EdgeIndexObj::getTail(PointCache& point_cache) {
    return point_cache[p[0]].getPos();
}

vec4 EdgeIndexObj::getTip(PointCache& point_cache) {
    return point_cache[p[1]].getPos();
}

vec4 EdgeIndexObj::calcEdgeVec(PointCache& point_cache) {
    vec4 tip_vec  = getTip(point_cache);
    vec4 tail_vec = getTail(point_cache);
    return vec4::subtrK(tip_vec, tail_vec, 3);
}

float EdgeIndexObj::calcLength(PointCache& point_cache) {
    vec4 edge_vec = calcEdgeVec(point_cache);
    return edge_vec.norm2K(3);
}

// ======== TriIndexObj ========
TriIndexObj::TriIndexObj(unsigned int v0, unsigned int v1, unsigned int v2)
    :v{ v0, v1, v2 } {}

vec4 TriIndexObj::getPos(int i, VertexCache& vertex_cache)
    { return vertex_cache[v[i]].getPos(); }
vec4 TriIndexObj::getNorm(int i, VertexCache& vertex_cache)
    { return vertex_cache[v[i]].getNorm(); }
vec2 TriIndexObj::getUV(int i, VertexCache& vertex_cache)
    { return vertex_cache[v[i]].getUV(); }

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