// #include "edit_space/edit_edge.hpp"
#include "em_sub_objects/edit_edge.hpp"

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