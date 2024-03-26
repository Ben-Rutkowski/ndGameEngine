#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "math/matrix.hpp"

const vec3 TRUE_UP = vec3({0.0f, 1.0f, 0.0f});

class Camera {
protected:
    // --- Camera Aspects ---
    float aspect_ratio;
    float fov;       // Vertical angle
    float near, far; // Near and far plane
    float height, width;
     
    // --- State ---
    vec4 position; // Position in world space
    vec3 pyr;      // pitch-yaw-roll

    // --- Cached Values ---
    vec3 front, right, up;
    mat4 view_M, orth_M, proj_M;

public:
    Camera(float near, float far, float fov, float aspect_ratio);
    void resetFieldOfView(float fov_in);
    void resetAspectRatio(float width_in, float height_in);

    // mat4 getView() { return view_M; } 
    // mat4 getOrth() { return orth_M; }
    // mat4 getProj() { return proj_M; }

    mat4 getOrth() { return orth_M*view_M; }
    mat4 getProj() { return proj_M*view_M; }

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
    mat4 calcOrth();
    mat4 calcProj();

// --- Debug ---
public:
    void debug();
};

#endif
