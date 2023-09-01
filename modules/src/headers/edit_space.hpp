#ifndef EDIT_SPACE_HPP
#define EDIT_SPACE_HPP

#include "event_manager.hpp"
#include "camera.hpp"
#include "edit_mesh.hpp"

#define EDIT_SPACE_SHADER_SUB_DIR "modules/src/shaders/edit_mesh/"

/* CLASS: EditCamera
An edit Camera is a camera that is always facing an origin
point. It can rotate around the origin, zoom in and out, and
pan the origin. The origin exists in the xz-plane.

The Camera is controlled by pitchYaw. Each time it is rotated,
the position is calculated so the camera is looking at the origin.
The distance is how far the camera is from the origin.
*/

class EditCamera : public Camera {
private:
    vec4  origin;
    float distance;

public:
    EditCamera();

    // vec2 ( delta x, delta z )
    void transOrigin(vec2 trans);
    void zoom(float delta);
    void rotate(float pitch_theta, float yaw_theta);
    void rotateInc(float pitch_delta, float yaw_delta);

private:
    void placePosition();

};

/* CLASS: EditSpace
*/

class EditSpace {
private: static Module module_name;
private:
    EventInterface event_interface;
    ShaderProgram  point_shader;
    ShaderProgram  line_shader;
    ShaderProgram  face_shader;

// --- Layers ---
private:
    EditCamera camera;

// --- Attributes ---
private:
    std::vector<EditMesh> meshes;

public:
    EditSpace();
    void setManagerPtr(EventManager* ptr);

// --- Editing ---
public:
    Id createMesh();
    Id createDefaultCube();
    EditMesh& ref(Id);

// --- Rendering ---
private:
    void load();
    void draw();

// --- Event Interface ---
public:
    void runEventEditSpace(Event* event);

private:
    void setCallbacks();

// --- On Events ---
private:
    void onBeginLoop(Event* event);
    void onStartFrame(Event* event);
    void onDrawFrame(Event* event);
    void onEndFrame(Event* event);

};

#endif