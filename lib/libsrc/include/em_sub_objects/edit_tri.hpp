#ifndef EDIT_TRI_HPP
#define EDIT_TRI_HPP

#include "em_sub_objects/edit_vertex.hpp"

/* CLASS: TriIndexObj
Contains IDs for EditVertexs in the VertexCache.
Users should not touch or create TriIndexObjects.
*/

class TriIndexObj {
private:
    Id vertex_ids[3];

public:
    TriIndexObj(Id v0_in, Id v1_in, Id v2_in);

    EditVertex& vert(int i, VertexCache& vc);
    Id vertId(int i);

// --- Calculations ---
public:
    vec4 calcNorm(VertexCache& vc);
};

/* CLASS: TriCache
*/

class TriCache : public EditCache<TriIndexObj> {
public:
    Id addTri(TriIndexObj tri) 
        { return addData(tri); }

    Id addTri(Id3 vert_ids) {
        TriIndexObj tri(vert_ids[0], vert_ids[1], vert_ids[2]);
        return addData(tri);
    }

    Id addTri(Id v0, Id v1, Id v2) {
        TriIndexObj tri(v0, v1, v2);
        return addData(tri);
    }

    int indexLen() {
        return 3*data.size();
    }
};

#endif