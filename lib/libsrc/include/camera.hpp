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

    mat4 getView();
    mat4 getProj();
    vec4 getFront();

// --- Controls ---
public:
    void grab(float x, float y);
    vec2 moveMouse(float x, float y);

    void translate(vec4 trans);
    void move(float f, float r, float u);
    void pitchYaw(float pitch_theta, float yaw_theta);
    void pitchYawInc(float pitch_delta, float yaw_delta);

    void calcView();
    void calcProj(float ratio_in);
    void setProj(float fov_in, float near_in, float far_in);

protected:
    void setFront(vec4 new_front);

private:
    // new_front must be normalized
    bool validFront(vec4 new_front);
    bool validPitch(float pitch_test);
    void calcPitchYaw();
    void trimYaw();
};

#endif