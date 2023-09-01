#include "edit_space.hpp"

EditCamera::EditCamera()
    :origin({0.0f, 0.0f, 0.0f, 1.0f}),
    distance{ 4.0f } {

    placePosition();
}

void EditCamera::transOrigin(vec2 trans) {
    origin.set(origin[0] + trans[0], 0);
    origin.set(origin[2] + trans[1], 0);
}

void EditCamera::zoom(float delta) {
    distance = delta;
    placePosition();
}

void EditCamera::rotate(float pitch_theta, float yaw_theta) {
    pitchYaw(pitch_theta, yaw_theta);
    placePosition();
}

void EditCamera::rotateInc(float pitch_delta, float yaw_delta) {
    pitchYawInc(pitch_delta, yaw_delta);
    placePosition();
}

// === Private ===
void EditCamera::placePosition() {
    vec4 rel_position = front.multK(-distance, 3);
    position = vec4::sumK(origin, rel_position, 3);
}