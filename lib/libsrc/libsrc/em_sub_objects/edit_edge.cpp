#include "em_sub_objects/edit_edge.hpp"

EdgeIndexObj::EdgeIndexObj(Id tail, Id tip)
    :point_ids{ tail, tip } {}

EditPoint& EdgeIndexObj::point(int i, PointCache& pc) {
    return pc[point_ids[i]];
}

Id EdgeIndexObj::pointId(int i) {
    return point_ids[i];
}

Id EdgeIndexObj::otherId(Id point_id) {
    if (point_ids[0] == point_id) {
        return point_ids[1];
    } else {
        return point_ids[0];
    }
}

// === Calculations ===
vec4 EdgeIndexObj::calcEdgeVec(PointCache& pc) {
    vec4 tail_pos = point(0, pc).getPos();
    vec4 tip_pos  = point(1, pc).getPos();
    return vec4::subtrK(tip_pos, tail_pos, 3);
}

float EdgeIndexObj::calcLength(PointCache& pc) {
    vec4 edge_vec = calcEdgeVec(pc);
    return edge_vec.norm2K(3);
}