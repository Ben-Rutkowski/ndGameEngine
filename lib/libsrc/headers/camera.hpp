#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "math/matrix.hpp"

const vec3 TRUE_UP = vec3({0.0f, 1.0f, 0.0f});

class Camera {
protected:
    // --- Camera Aspects ---
    float near; // Near Plane
    float far;  // Far Plane
    float fov;  // Vertical angle
    float width, height;
     
    // --- State ---
    vec4 position; // Position in world space
    vec3 pyr;      // pitch-yaw-roll

    // --- Cached Values ---
    vec3 front, right, up;
    mat4 view_M, orth_M, proj_M;

public:
    Camera(float near, float far, float fov, float width, float height);
    void resetFieldOfView(float fov);
    void resetAspectRatio(float width, float height);

    mat4 getView() { return view_M; } 
    mat4 getOrth() { return orth_M; }
    mat4 getProj() { return proj_M; }

//    --- Controls ---
public:
    void setPosition(vec4 new_pos);
    void lookAt(vec4 focus);
    
    void nudgePYR(vec3 delta_pyr);

//    --- Calculations ---
private:
    void recalcAxes();
    void recalcMats();

    mat4 calcView();
    mat4 calcOrht();
    mat4 calcProj();

// --- Debug ---
public:
    void debug();
};

#endif
