#ifndef EDIT_TRI_HPP
#define EDIT_TRI_HPP

#include "em_sub_objects/edit_vertex.hpp"

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
    unsigned int getVertId(int i);

    vec4 calcNorm(VertexCache& vertex_cache);
};

/* CLASS: TriCache
*/

class TriCache : public EditCache<TriIndexObj> {
public:
    unsigned int addTri(TriIndexObj tri) 
        { return addData(tri); }

    int indexLen() {
        return 3*data.size();
    }
};

#endif