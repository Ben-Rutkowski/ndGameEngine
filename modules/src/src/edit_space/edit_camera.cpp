#include "edit_space.hpp"

EditCamera::EditCamera()
    :origin({0.0f, 0.0f, 0.0f, 1.0f}),
    distance{ 4.0f } {}

void EditCamera::transOrigin(vec2 trans) {
    origin.set(origin[0] + trans[0], 0);
    origin.set(origin[2] + trans[1], 0);
}

void EditCamera::zoom(float delta) {
    distance = distance + delta;
}

// === Private ===
void EditCamera::placePosition() {
    vec4 rel_position = front.multK(-distance, 3);
    position = vec4::sumK(origin, rel_position, 3);
}