#include "camera.hpp"
#include "math/vector.hpp"
#include <cmath>
#include <iostream>

const float CON   = M_PI/360.0f;

Camera::Camera(float near_, float far_, float fov_, float aspect_ratio_)
    :aspect_ratio{ aspect_ratio_ }, fov{ fov_*CON }, near{ near_ }, far{ far_ },
    height{ near*tanf(fov) }, width{ aspect_ratio*height },
    position(0.0f), pyr(0.0f)
{
    recalcAxes();
    recalcMats();
}

void Camera::resetFieldOfView(float fov_in) {
    fov    = fov_in*CON;
    height = near*tanf(fov);
    width  = aspect_ratio*height;

    orth_M = calcOrth();
    proj_M = calcProj();
}

void Camera::resetAspectRatio(float width_in, float height_in) {
    aspect_ratio = width_in/height_in;
    width        = aspect_ratio*height;

    orth_M = calcOrth();
    proj_M = calcProj();
}


// ================ Controls ================
void Camera::setPosition(vec4 new_pos) {
    position = new_pos;
    recalcAxes();
    recalcMats();
}


// ================ Calculations ================
void Camera::recalcAxes() {
    front = vec3({
        -cosf(pyr[0])*sinf(pyr[1]),
         sinf(pyr[0]),
        -cosf(pyr[0])*cosf(pyr[1])
    }); 

    right = front.cross(TRUE_UP);
    right.normalizeL2();
    up    = right.cross(front);
}

void Camera::recalcMats() {
    view_M = calcView();
    orth_M = calcOrth();
    proj_M = calcProj();
}

mat4 Camera::calcView() {
    mat4 translate = mat4({
         1.0f,         0.0f,         0.0f,        0.0f,
         0.0f,         1.0f,         0.0f,        0.0f,
         0.0f,         0.0f,         1.0f,        0.0f,
        -position[0], -position[1], -position[2], 1.0f,
    });

    mat4 rotation = mat4({
        right[0], up[0], -front[0], 0.0f,
        right[1], up[1], -front[1], 0.0f,
        right[2], up[2], -front[2], 0.0f,
        0.0f,     0.0f,   0.0f,     1.0f
    });

    return rotation*translate;
}

mat4 Camera::calcOrth() { 
    float t1 = 1.0f/aspect_ratio;
    float t2 = 1.0f;
    float t3 = 1.0f/(near-far);
    float t4 = near/(near-far);

    return mat4({
        t1,   0.0f, 0.0f, 0.0f,
        0.0f, t2,   0.0f, 0.0f,
        0.0f, 0.0f, t3,   0.0f,
        0.0f, 0.0f, t4,   1.0f
    });
}

mat4 Camera::calcProj() { 
    float t1 = near/width;
    float t2 = near/height;
    float t3 = far/(near-far);
    float t4 = near*far/(near-far);

    return mat4({
        t1,   0.0f, 0.0f,  0.0f,
        0.0f, t2,   0.0f,  0.0f,
        0.0f, 0.0f, t3,   -1.0f,
        0.0f, 0.0f, t4,    0.0f
    });
}


// ================ Debug ================
void Camera::debug() {
    std::cout << "CAMERA--DEBUG" << std::endl;

    std::cout << "Near: "  << near;
    std::cout << ", Far: " << far;
    std::cout << ", FOV: " << fov;
    std::cout << ", Width: "  << width;
    std::cout << ", Height: " << height << std::endl;

    std::cout << "Positon: "; position.debug();
    std::cout << "PYR: ";     pyr.debug();
    std::cout << "Front: ";   front.debug();
    std::cout << "Right: ";   right.debug();
    std::cout << "Up: ";      up.debug();

    std::cout << "View Matrix: \n";         view_M.debug();
    std::cout << "Orthographic Matrix: \n"; orth_M.debug();
    std::cout << "Projection Matrix: \n";   proj_M.debug();
}
