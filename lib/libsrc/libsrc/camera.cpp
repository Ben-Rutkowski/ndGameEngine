#include "camera.hpp"

Camera::Camera()
    :fov{ math::rads(45.0f) },
    near{ 0.1f },
    far{ 100.0f },
    ratio{ 800.0f/600.0f },
    position({0.0f, 0.0f, 1.0f, 1.0f}),
    front({0.0f, 0.0f, -1.0f, 1.0f}),
    right({1.0f, 0.0f, 0.0f, 1.0f}),
    up({0.0f, 1.0f, 0.0f, 1.0f}),
    view{ mat4::view(position, front, right) },
    proj{ mat4::projPer(fov, ratio, near, far) } {}

// === Move ===
void Camera::translate(vec4 trans) {
    position = vec4::sumK(position, trans, 3);
}

void Camera::move(float f, float r, float u) {
    vec4 true_up = vec4::basis(1);
    vec4 trans = front.multK(f, 3) + right.multK(r, 3) + true_up.multK(u, 3);
    translate(trans);
}

void Camera::pitchYaw(float sin_vert, float sin_horz) {
    vec4 new_front;
    float cos_vert = sqrt(1 - sin_vert*sin_vert);
    float cos_horz = sqrt(1 - sin_horz*sin_horz);

    new_front = vec4::weightK(cos_vert, front, sin_vert, up, 3);
    new_front = vec4::weightK(cos_horz, new_front, sin_horz, right, 3);
    
    setFront(new_front);
}

void Camera::calcView() {
    view = mat4::view(position, front, right);
}

void Camera::calcProj(float ratio) {
    proj = mat4::projPer(fov, ratio, near, far);
}

void Camera::setProj(float fov_in, float near_in, float far_in) {
    fov  = fov_in;
    near = near_in;
    far  = far_in;
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
    return 1.0f-abs(new_front[2]) > 0.001f;
}