#include "camera.hpp"

Camera::Camera()
    :fov{math::rads(45.0f)},
    near{0.1f},
    far{100.0f},
    pitch{0.0f},
    yaw{0.0f},
    ratio{800.0f/600.0f},
    position({0.0f, 0.0f, 1.0f, 1.0f}),
    front({0.0f, 0.0f, -1.0f, 1.0f}),
    right({1.0f, 0.0f, 0.0f, 1.0f}),
    up({0.0f, 1.0f, 0.0f, 1.0f}),
    view{mat4::view(position, front, right)},
    proj{mat4::projPer(fov, ratio, near, far)} {}

mat4  Camera::getView()  { return view; }
mat4  Camera::getProj()  { return proj; }
vec4  Camera::getFront() { return front; }
vec4  Camera::getPos()   { return position; }
vec4  Camera::getRight() { return right; }
vec4  Camera::getUp()    { return up; }
float Camera::getNear()  { return near; }
float Camera::getProjH() { return near*tan(fov); }
float Camera::getProjW() { return getProjH()*ratio; }

CameraPack Camera::getPack() {
    return CameraPack {
        front, right, position, view, proj
    };
}

// === Mouse ===
void Camera::grab(float x, float y) {
    mouse_x = x;
    mouse_y = y;
}

vec2 Camera::moveMouse(float x, float y) {
    vec2 delta({x - mouse_x, y - mouse_y});
    mouse_x = x;
    mouse_y = y;
    return delta;
}

// === Translate ===
void Camera::translate(vec4 trans) {
    position = vec4::sumK(position, trans, 3);
}

void Camera::pitchYaw(float pitch_theta, float yaw_theta) {
    if (validPitch(pitch_theta)) { pitch = pitch_theta; }
    if (validYaw(yaw_theta))     { yaw   = yaw_theta; }
    calcPitchYaw();
}

void Camera::calcPitchYaw() {
    const float FULL_CIRCLE = 2.0f * M_PI;
    const vec4  TRUE_FRONT({0.0f, 0.0f, -1.0f, 1.0f});

    if (abs(yaw) > FULL_CIRCLE) {
        yaw = math::remander(yaw, FULL_CIRCLE);
    }

    mat4 rotx = mat4::rotX(pitch);
    mat4 roty = mat4::rotY(yaw);

    vec4 new_front = roty * rotx * TRUE_FRONT;
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
    fov = fov_in;
    near = near_in;
    far = far_in;

    calcProj(ratio);
}

// === Selecting ===
// mat4 Camera::selectMatProj(mat4 model, vec2 br, vec2 tl) {
//     float h = near*tan(fov);
//     float w = ratio*h;
//     return mat4::selectProjPartial(br, tl, w, h, near)*view*model;
// }

vec4 Camera::clipToWorld(vec2 v) {
    float h = near*tan(fov);
    float w = ratio*h;

    vec4 r = right.multK(v[0]*w, 3);
    vec4 u = up.multK(v[1]*h, 3);
    vec4 f = front.multK(near, 3);

    vec4 out = vec4::sumK(r, u, 3);
    out = vec4::sumK(out, f, 3);
    return vec4::sumK(out, position, 3);
}

// === Protected ===
void Camera::setFront(vec4 new_front) {
    new_front.normalizeK(3);
    if (validFront(new_front)) {
        front = new_front;
        right = vec4::rightVec(front);
        up = right.cross(front);
        up.normalizeK(3);
    }
}

// === Debugging ===
void Camera::print() {
    std::cout << "Front Vec"; front.print();
    std::cout << "Pitch: " << pitch << " Yaw: " << yaw << std::endl;
    std::cout << "Mouse x: " << mouse_x << " Mouse y: " << mouse_y << std::endl << std::endl; 
}

// === Private ===
bool Camera::validFront(vec4 new_front) {
    return
        new_front.isFinite() && (new_front[0] != 0.0f || new_front[2] != 0.0f);
}

bool Camera::validPitch(float pitch_test) {
    const float BOUND = M_PI/2.0f - 0.001f;
    return (isfinite(pitch_test) && abs(pitch_test) < BOUND);
}

bool Camera::validYaw(float yaw_test) {
    const float BOUND = 4.0f*M_PI;
    return ( isfinite(yaw_test) && abs(yaw_test) < BOUND );
}