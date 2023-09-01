#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "matrix.hpp"

class Camera {
// --- Attributes ---
protected:
    float fov;
    float near;
    float far;
    float ratio;

    vec4 position;
    vec4 front;
    vec4 right;
    vec4 up;

    mat4 view;
    mat4 proj;

// --- Constructor ---
public:
    Camera();

// --- Controls ---
public:
    void translate(vec4 trans);
    void move(float f, float r, float u);
    void pitchYaw(float sin_vert, float sin_horz);

    void calcView();
    void calcProj(float ratio);
    void setProj(float fov_in, float near_in, float far_in);

protected:
    void setFront(vec4 new_front);

private:
    // new_front must be normalized
    bool validFront(vec4 new_front);
};

#endif