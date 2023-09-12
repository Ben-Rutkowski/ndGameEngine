#ifndef EDIT_CAMERA_HPP
#define EDIT_CAMERA_HPP

#include "math/vector.hpp"
#include "camera.hpp"

/* CLASS: EditCamera
An edit Camera is a camera that is always facing an origin
point. It can rotate around the origin, zoom in and out, and
pan the origin. The origin exists in the xz-plane.

The Camera is controlled by pitchYaw. Each time it is rotated,
the position is calculated so the camera is looking at the origin.
The distance is how far the camera is from the origin.
*/

#define MOUSE_DISTANCE_FACTOR 1.2f

class EditCamera : public Camera {
private:
    vec4  origin;
    vec4  plane_front;
    float distance;

public:
    EditCamera();

    void transOrigin(vec2 delta);
    void zoom(float delta);

    void scroll(float offset);
    void rightClick(float x_pos, float y_pos);
    void rightDrag(float x_pos, float y_pos);

    void setRotate(float pitch_theta, float yaw_theta);
    void setDistance(float dist);

private:
    void  calcParams();
    bool  validDistance(float dist);
    float boundedDelta(float delta);
};

#endif