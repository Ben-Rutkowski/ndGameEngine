#include "camera.hpp"

Camera::Camera()
    :fov{ math::rads(45.0f) },
    near{ 0.1f },
    far{ 100.0f },
    pitch{ 0.0f },
    yaw{ 0.0f },
    ratio{ 800.0f/600.0f },
    position({0.0f, 0.0f, 1.0f, 1.0f}),
    front({0.0f, 0.0f, -1.0f, 1.0f}),
    right({1.0f, 0.0f, 0.0f, 1.0f}),
    up({0.0f, 1.0f, 0.0f, 1.0f}),
    view{ mat4::view(position, front, right) },
    proj{ mat4::projPer(fov, ratio, near, far) } {}

mat4 Camera::getView() { return view; }
mat4 Camera::getProj() { return proj; }

// === Move ===
void Camera::translate(vec4 trans) {
    position = vec4::sumK(position, trans, 3);
}

void Camera::move(float f, float r, float u) {
    vec4 true_up = vec4::basis(1);
    vec4 trans = front.multK(f, 3) + right.multK(r, 3) + true_up.multK(u, 3);
    translate(trans);
}

void Camera::pitchYawInc(float pitch_delta, float yaw_delta) {
    if ( validPitch(pitch+pitch_delta) ) {
        pitch = pitch + pitch_delta;
        yaw = yaw + yaw_delta;
        calcPitchYaw();
    } else {
        yaw = yaw + yaw_delta;
        calcPitchYaw();
    }
}

void Camera::pitchYaw(float pitch_theta, float yaw_theta) {
    pitch = pitch_theta;
    yaw   = yaw_theta;
    calcPitchYaw();
}

void Camera::calcPitchYaw() {
    const float FULL_CIRCLE = 2.0f*M_PI;
    const vec4  TRUE_FRONT({0.0f, 0.0f, -1.0f, 1.0f});

    if ( abs(yaw) > FULL_CIRCLE ) { yaw = yaw - FULL_CIRCLE; }
    mat4 rotx = mat4::rotX(pitch);
    mat4 roty = mat4::rotY(yaw);

    vec4 new_front = roty*rotx*TRUE_FRONT;    
    setFront(new_front);
}

void Camera::calcView() {
    view = mat4::view(position, front, right);
}

void Camera::calcProj(float ratio_in) {
    ratio = ratio_in;
    proj = mat4::projPer(fov, ratio_in, near, far);
}

void Camera::setProj(float fov_in, float near_in, float far_in) {
    fov  = fov_in;
    near = near_in;
    far  = far_in;

    calcProj(ratio);
}

// === Protected ===
void Camera::setFront(vec4 new_front) {
    new_front.normalizeK(3);
    if (validFront(new_front)) {
        front = new_front;
        right = vec4::rightVec(front);
        up    = right.cross(front);
        up.normalizeK(3);
    }
}

// === Private ===
bool Camera::validFront(vec4 new_front) {
    return (new_front[0] != 0.0f || new_front[0] != 0.0f);
}

bool Camera::validPitch(float pitch_test) {
    const float BOUND = M_PI/2.0f - 0.001f;
    return ( -BOUND < pitch_test && pitch_test < BOUND );
}

void Camera::trimYaw() {
    
}