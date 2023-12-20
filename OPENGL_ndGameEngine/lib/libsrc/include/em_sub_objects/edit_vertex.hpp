#ifndef EDIT_VERTEX_HPP
#define EDIT_VERTEX_HPP

#include "em_sub_objects/edit_cache.hpp"

/* CLASS: EditVertex

An Edit Vertex Contains a position, normal, uv coordinates.
An EditVertex can be constructed with just a position, and
default normal and uv, or with a position, normal and uv.

*/

class EditVertex {
// ---- Attributes ----
private:
    vec4  position;
    vec4  normal;
    vec4  center;
    vec2  uv;
    float select;

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
    bool getSelect();

    void setPos(vec4 pos_in);
    void setNorm(vec4 norm_in);
    void setCenter(vec4 cent_in);
    void setUV(vec2 uv_in);
    void setSelect(bool value);
};

/* CLASS: VertexCache
A VertexCache contains EditVertexs as its data and does not utilize any caches. 
Cache 1: Points
*/

class VertexCache : public EditCache<EditVertex> {
public:
    Id addVertex(EditVertex vertex) {
        return addData(vertex);
    }

    Id addVertex(vec4 position) {
        EditVertex vertex(position);
        return addData(vertex);
    }
};

#endif