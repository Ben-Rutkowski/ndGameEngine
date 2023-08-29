#ifndef EDIT_INDEX_OBJECT_HPP
#define EDIT_INDEX_OBJECT_HPP

#include "vector"
#include "edit_cache.hpp"

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

    void pairFace(unsigned int edge_id, unsigned int face_id)
        { pairIndexCache1(edge_id, face_id); }

    int faceNum(unsigned int edge_id)
        { return cacheNum1(edge_id); }

    unsigned int getFace(unsigned int edge_id, int i)
        { return getCache1(edge_id, i); }
};


/* CLASS: TriIndexObj
Contains IDs for EditVertexs in the VertexCache.
Users should not touch or create TriIndexObjects.
*/

class TriIndexObj {
private:
    unsigned int v[3];

public:
    TriIndexObj(unsigned int v0_in, unsigned int v1_in, unsigned int v2_in);
    vec4 getPos(int i, VertexCache& vertex_cache);
    vec4 getNorm(int i, VertexCache& vertex_cache);
    vec2 getUV(int i, VertexCache& vertex_cache);

    vec4 calcNorm(VertexCache& vertex_cache);
};

/* CLASS: TriCache
*/

class TriCache : public EditCache<TriIndexObj> {
public:
    unsigned int addTri(TriIndexObj tri) 
        { return addData(tri); }
};

#endif