#ifndef EDIT_EDGE_HPP
#define EDIT_EDGE_HPP

#include "em_sub_objects/edit_point.hpp"

/* CLASS: EdgeIndexObj
*/

class EdgeIndexObj {
private:
    Id point_ids[2]; // { tail, tip }

public:
    EdgeIndexObj(Id tail_in, Id tip_in);
    EditPoint& point(int i, PointCache& pc);
    Id pointId(int i);
    Id otherId(Id point_id);

// --- Calculations ---
public:
    vec4  calcEdgeVec(PointCache& pc);
    float calcLength(PointCache& pc);
};

/* CLASS: EdgeCache
Data : EdgeIndexObj
Cache1 : Faces
*/

class EdgeCache : public EditCache<EdgeIndexObj> {
public:
    Id addEdge(EdgeIndexObj edge) {
        pushPairCache1(2);
        return addData(edge);
    }

    int indexLen() { return 2*dataLen(); }

    void pairFace(Id edge_id, Id face_id)
        { pairIndexCache1(edge_id, face_id); }

    int pairedFaceLen(Id edge_id)
        { return pairedLen1(edge_id); }

    Id getPairedFace(Id edge_id, int i)
        { return getPairedCache1(edge_id, i); }
};

#endif