#ifndef EDIT_CACHE_HPP
#define EDIT_CACHE_HPP

#include "vector.hpp"

/* CLASS TEMPLATE: EditCache

An EditCache contains a stack of objects. An EditCache also 
contains references to every edge and every face that contains
the point.

*/

template<typename T>
class EditCache {
protected:
    std::vector<T> data;
    std::vector<std::vector<unsigned int>> index_cache_1;
    std::vector<std::vector<unsigned int>> index_cache_2;

public:
    T&    operator[](int i) { return data[i]; }
    void* dataPtr()  { return (void*)&data[0]; }    
    int   dataSize() { return data.size()*sizeof(T); }
    int   dataLen()  { return data.size(); }

protected:
    unsigned int addData(T& data_object) {
        data.push_back(data_object);
        return data.size() - 1;
    }

    unsigned int createData() {
        data.push_back(T());
        return data.size() - 1;
    }

    void pushCache1() { index_cache_1.push_back(std::vector<unsigned int>()); }
    void pushCache2() { index_cache_2.push_back(std::vector<unsigned int>()); }

    void pairIndexCache1(unsigned int root_id, unsigned int pair_id) 
        { index_cache_1[root_id].push_back(pair_id); }
    void pairIndexCache2(unsigned int root_id, unsigned int pair_id)
        { index_cache_2[root_id].push_back(pair_id); }    

    int cacheNum1(int data_id) { return index_cache_1[data_id].size(); }
    int cacheNum2(int data_id) { return index_cache_2[data_id].size(); }

    unsigned int getCache1(int data_id, int i) { return index_cache_1[data_id][i]; }
    unsigned int getCache2(int data_id, int i) { return index_cache_2[data_id][i]; }
};

class EditPoint {
// ---- Attributes ----
private:
    vec4 position;

public:
    EditPoint(vec4 pos_in);
    vec4 getPos();
    void setPos(vec4 pos_in);
};

/* CLASS: PointCache

A PointCache has as data vec4 for positions and has edge and face indices.

*/

class PointCache : public EditCache<EditPoint> {
public:
    unsigned int addPoint(vec4 position) {
        pushCache1();
        pushCache2();
        EditPoint point(position);
        return addData(point);
    }

    void pairEdge(unsigned int point_id, unsigned int edge_id) 
        { pairIndexCache1(point_id, edge_id); }
    void pairFace(unsigned int point_id, unsigned int face_id) 
        { pairIndexCache2(point_id, face_id); }

    int edgeNum(unsigned int point_id) { return cacheNum1(point_id); }
    int faceNum(unsigned int point_id) { return cacheNum2(point_id); }

    unsigned int getEdge(unsigned int point_id, int i)
        { return getCache1(point_id, i); }
    unsigned int getFace(unsigned int point_id, int i)
        { return getCache2(point_id, i); }
};

/* CLASS: EditVertex

An Edit Vertex Contains a position, normal, uv coordinates.
An EditVertex can be constructed with just a position, and
default normal and uv, or with a position, normal and uv.

*/

class EditVertex {
// ---- Attributes ----
private:
    vec4 position;
    vec4 normal;
    vec4 center;
    vec2 uv;

// ---- Constructor ----
public:
    EditVertex(vec4 pos_in);
    EditVertex(vec4 pos_in, vec4 norm_in, vec2 uv_in);

// ---- Gets and Sets ----
public:
    vec4 getPos();
    vec4 getNorm();
    vec4 getCenter();
    vec2 getUV();

    void setPos(vec4 pos_in);
    void setNorm(vec4 norm_in);
    void setCenter(vec4 cent_in);
    void setUV(vec2 uv_in);
};

/* CLASS: VertexCache
A VertexCache contains EditVertexs as its data and does not utilize any caches. 
*/

class VertexCache : public EditCache<EditVertex> {
public:
    unsigned int addVertex(EditVertex vertex) {
        return addData(vertex);
    }

    unsigned int addVertex(vec4 position) {
        EditVertex vertex(position);
        return addData(vertex);
    }
};

#endif