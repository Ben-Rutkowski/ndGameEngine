#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "matrix.hpp"

class Camera {
// --- Attributes ---
protected:
    float fov;
    float near;
    float far;
    float pitch;
    float yaw;
    float ratio;

    float mouse_x;
    float mouse_y;

    vec4 position;
    vec4 front;
    vec4 right;
    vec4 up;

    mat4 view;
    mat4 proj;

// --- Constructor ---
public:
    Camera();

    mat4  getView();
    mat4  getProj();
    vec4  getFront();
    vec4  getPos();
    float getNear();
    float getProjW();
    float getProjH();

// --- Mouse ---
public:
    void grab(float x, float y);
    vec2 moveMouse(float x, float y);

// --- Move ---
    void translate(vec4 trans);
    void pitchYaw(float pitch_theta, float yaw_theta);
    // void pitchYawInc(float pitch_delta, float yaw_delta);

    void calcView();
    void calcProj(float ratio_in);
    void setProj(float fov_in, float near_in, float far_in);

// --- Selecting ---
public:
    mat4 selectMatProj(mat4 model, vec2 br, vec2 tl);

// --- Debugging ---
public:
    void print();

protected:
    void setFront(vec4 new_front);

private:
    // new_front must be normalized
    bool validFront(vec4 new_front);
    bool validPitch(float pitch_test);
    bool validYaw(float yaw_test);
    void calcPitchYaw();
};

#endif