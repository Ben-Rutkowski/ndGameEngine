#include "em_sub_objects/edit_point.hpp"

EditPoint::EditPoint(vec4 pos_in)
    :position{ pos_in },
    select{ 0.0f } {}

// === Gets and Sets ===
vec4 EditPoint::getPos() { return position; }
bool EditPoint::getSelect() { 
    if (select == 0.0f) {
        return false;
    } else {
        return true;
    }
}
void EditPoint::setPos(vec4 pos_in) { position = pos_in; }
void EditPoint::setSelect(bool value) {
    if (value) {
        select = 1.0f;
    } else {
        select = 0.0f;
    }
}