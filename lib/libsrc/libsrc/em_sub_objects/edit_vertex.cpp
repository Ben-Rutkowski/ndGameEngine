#include "em_sub_objects/edit_vertex.hpp"

EditVertex::EditVertex(vec4 pos_in)
    :position{ pos_in } {}

EditVertex::EditVertex(vec4 pos_in, vec4 norm_in, vec2 uv_in)
    :position{ pos_in },
    normal{ norm_in },
    uv{ uv_in },
    select{ 0.0f } {}

// === Gets and Sets ===
vec4 EditVertex::getPos()    { return position; }
vec4 EditVertex::getNorm()   { return normal; }
vec4 EditVertex::getCenter() { return center; }
vec2 EditVertex::getUV()     { return uv; }

void EditVertex::setPos(vec4 pos_in)     { position = pos_in; }
void EditVertex::setNorm(vec4 norm_in)   { normal = norm_in; }
void EditVertex::setCenter(vec4 cent_in) { center = cent_in; }
void EditVertex::setUV(vec2 uv_in)       { uv = uv_in; }
void EditVertex::setSelect(bool value) {
    if (value) {
        select = 1.0f;
    } else {
        select = 0.0f;
    }
}