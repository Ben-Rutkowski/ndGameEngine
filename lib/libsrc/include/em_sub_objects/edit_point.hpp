#ifndef EDIT_POINT_HPP
#define EDIT_POINT_HPP

#include "em_sub_objects/edit_cache.hpp"

class EditPoint {
// ---- Attributes ----
private:
    vec4  position;
    float select;

public:
    EditPoint(vec4 pos_in);
    vec4 getPos();
    bool getSelect();
    void setPos(vec4 pos_in);
    void setSelect(bool value);
};

/* CLASS: PointCache

A PointCache has as data vec4 for positions and has edge and face indices.

*/

class PointCache : public EditCache<EditPoint> {
public:
    unsigned int addPoint(vec4 position) {
        pushCache1();
        pushCache2();
        pushCache3();
        EditPoint point(position);
        return addData(point);
    }

    void pairEdge(unsigned int point_id, unsigned int edge_id) 
        { pairIndexCache1(point_id, edge_id); }
    void pairFace(unsigned int point_id, unsigned int face_id) 
        { pairIndexCache2(point_id, face_id); }
    void pairVertex(unsigned int point_id, unsigned int vertex_id) 
        { pairIndexCache3(point_id, vertex_id); }

    int edgeNum(unsigned int point_id)   { return cacheNum1(point_id); }
    int faceNum(unsigned int point_id)   { return cacheNum2(point_id); }
    int vertexNum(unsigned int point_id) { return cacheNum3(point_id); }

    unsigned int getEdge(unsigned int point_id, int i)
        { return getCache1(point_id, i); }
    unsigned int getFace(unsigned int point_id, int i)
        { return getCache2(point_id, i); }
    unsigned int getVertex(unsigned int point_id, int i)
        { return getCache3(point_id, i); }
};

#endif