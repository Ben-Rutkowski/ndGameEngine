#include "edit_cache.hpp"

EditPoint::EditPoint(vec4 pos_in)
    :position{ pos_in } {}

vec4 EditPoint::getPos() { return position; }
void EditPoint::setPos(vec4 pos_in) { position = pos_in; }

// ======== EditVertex ========
// === Constructor ===
EditVertex::EditVertex(vec4 pos_in)
    :position{ pos_in } {}

EditVertex::EditVertex(vec4 pos_in, vec4 norm_in, vec2 uv_in)
    :position{ pos_in },
    normal{ norm_in },
    uv{ uv_in } {}

// === Gets and Sets ===
vec4 EditVertex::getPos()  { return position; }
vec4 EditVertex::getNorm() { return normal; }
vec2 EditVertex::getUV()   { return uv; }

void EditVertex::setPos(vec4 pos_in)   { position = pos_in; }
void EditVertex::setNorm(vec4 norm_in) { normal = norm_in; }
void EditVertex::setUV(vec2 uv_in)     { uv = uv_in; }