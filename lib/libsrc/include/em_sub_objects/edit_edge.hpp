#ifndef EDIT_EDGE_HPP
#define EDIT_EDGE_HPP

#include "em_sub_objects/edit_point.hpp"

/* CLASS: EdgeIndexObj
*/

class EdgeIndexObj {
private:
    unsigned int p[2]; // { tail, tip }

public:
    EdgeIndexObj(unsigned int tail_in, unsigned int tip_in);
    vec4  getTail(PointCache& point_cache);
    vec4  getTip(PointCache& point_cache);
    vec4  calcEdgeVec(PointCache& point_cache);
    float calcLength(PointCache& point_cache);
};

/* CLASS: EdgeCache
Data : EdgeIndexObj
Cache1 : Faces
*/

class EdgeCache : public EditCache<EdgeIndexObj> {
public:
    unsigned int addEdge(EdgeIndexObj edge) {
        pushCache1();
        return addData(edge);
    }

    int indexLen() { return 2*dataLen(); }

    void pairFace(unsigned int edge_id, unsigned int face_id)
        { pairIndexCache1(edge_id, face_id); }

    int faceNum(unsigned int edge_id)
        { return cacheNum1(edge_id); }

    unsigned int getFace(unsigned int edge_id, int i)
        { return getCache1(edge_id, i); }
};

#endif