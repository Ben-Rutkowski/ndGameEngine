#include "edit_space.hpp"

EditCamera::EditCamera()
    :origin({0.0f, 0.0f, 0.0f, 1.0f}),
    distance{ 4.0f } {

    calcParams();
}

void EditCamera::transOrigin(vec2 delta) {
    const float FACTOR = 0.01f;

    vec4 trans = vec4::weightK(
        -FACTOR*delta[0], right, FACTOR*delta[1], plane_front, 3
    );
    mat4 tmat = mat4::translate(trans);
    origin = tmat*origin;
    calcParams();
}

void EditCamera::setDistance(float dist) {
    // if (validDistance(dist)) {
    //     distance = dist;
    //     calcParams();
    // }
    distance = dist;
    calcParams();
}

void EditCamera::setRotate(float pitch_theta, float yaw_theta) {
    pitchYaw(pitch_theta, yaw_theta);
    calcParams();
}

void EditCamera::zoom(float delta) {
    const float FACTOR = 0.25f;

    delta = boundedDelta(delta);
    float new_delta = distance - FACTOR*delta;
    if (validDistance(new_delta)) {
        distance = new_delta;
        calcParams();
    }
}

void EditCamera::rotateInc(float pitch_delta, float yaw_delta) {
    pitchYawInc(pitch_delta, yaw_delta);
    calcParams();
}

// === Private ===
void EditCamera::calcParams() {
    vec4 TRUE_UP({0.0f, 1.0f, 0.0f, 1.0f});

    vec4 rel_position = front.multK(-distance, 3);
    position = vec4::sumK(origin, rel_position, 3);

    plane_front = TRUE_UP.cross(right);
    plane_front.normalizeK(3);
}

bool EditCamera::validDistance(float dist) {
    const float BOUND = 250.0f;
    return 
        isfinite(dist) && (1.0f < dist) && (dist < BOUND);
}

float EditCamera::boundedDelta(float delta) {
    const float BOUND = 5.0f;

    if ( delta >= 0.0f ) {
        return std::min(delta, BOUND);
    } else {
        return std::max(delta, -BOUND);
    }
}