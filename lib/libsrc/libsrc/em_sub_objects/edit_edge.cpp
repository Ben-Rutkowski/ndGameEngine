#include "em_sub_objects/edit_edge.hpp"

EdgeIndexObj::EdgeIndexObj(Id tail, Id tip)
    :point_ids{ tail, tip } {}

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

void EdgeIndexObj::replacePoint(Id old_point_id, Id new_point_id) {
    if (point_ids[0] == old_point_id) {
        point_ids[0] = new_point_id;
    } else if (point_ids[1] == old_point_id) {
        point_ids[1] = new_point_id;
    }
}

// === Calculations ===
vec4 EdgeIndexObj::calcEdgeVec(PointCache& pc) {
    vec4 tail_pos = pc[pointId(0)].getPos();
    vec4 tip_pos  = pc[pointId(1)].getPos();
    return vec4::subtrK(tip_pos, tail_pos, 3);
}

float EdgeIndexObj::calcLength(PointCache& pc) {
    vec4 edge_vec = calcEdgeVec(pc);
    return edge_vec.norm2K(3);
}