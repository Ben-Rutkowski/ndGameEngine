#ifndef EDIT_SPACE_HPP
#define EDIT_SPACE_HPP

#define MOUSE_DISTANCE_FACTOR 250.0f

#include "camera.hpp"
#include "edit_mesh.hpp"
#include "select_box.hpp"
#include "nd_module.hpp"

#define EDIT_SPACE_SHADER_SUB_DIR "modules/src/shaders/edit_mesh/"
#define GRIDLINE_SHADER_SUB_DIR "modules/src/shaders/gridline/"
#define SELECT_BOX_SHADER_SUB_DIR "modules/src/shaders/select_box/"

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
    vec4  plane_front;
    float distance;

public:
    EditCamera();

    void transOrigin(vec2 delta);
    void setRotate(float pitch_theta, float yaw_theta);
    void setDistance(float dist);
    void zoom(float delta);
    void rotateInc(float pitch_delta, float yaw_delta);

private:
    void  calcParams();
    bool  validDistance(float dist);
    float boundedDelta(float delta);

};

/* CLASS GridLine 
*/

class GridLine {
private:
    VertexBufferInterface vbi;

    float length;
    vec4 direction;
    vec4 color;

    float offsets[10];
    vec4  blank_color;
    vec4  orthog;

public:
    GridLine(vec4 dir_in, vec4 color_in);
    void load(float length_in);
    void draw(ShaderProgram& program, mat4 view, mat4 proj);
    void drawPlane(ShaderProgram& program, ShaderProgram& plane_program, mat4 view, mat4 proj);
    void createPlane(vec4 blank_in, vec4 othog_in);
};

/* CLASS: EditSpace

    Opperation 1 : draw select box

*/

class EditSpace : public ndModule {
private:
    ShaderProgram  point_shader;
    ShaderProgram  line_shader;
    ShaderProgram  face_shader;
    ShaderProgram  gridline_shader;
    ShaderProgram  gridline_plane_shader;
    ShaderProgram  select_box_shader;
    ShaderProgram  select_box_faces_shader;

// --- Layers ---
private:
    EditCamera camera;
    GridLine x_line;
    GridLine y_line;
    GridLine z_line;

// --- Attributes ---
private:
    std::vector<EditMesh> meshes;
    SelectBox  select_box;

public:
    EditSpace();
    void setManagerPtr(EventManager* ptr);

// --- Editing ---
public:
    Id createMesh();
    Id createDefaultCube();
    EditMesh& ref(Id);

public:
    // void selectPoints(mat4 select_mat, Id mesh_id);

// --- Rendering ---
private:
    void load();
    void draw();

// --- Event Interface ---
public:
    void runEvent(Event* event);

private:
    void setCallbacks();

// --- On Events ---
private:
    void onBeginLoop(Event* event);
    void onStartFrame(Event* event);
    void onDrawFrame(Event* event);
    void onEndFrame(Event* event);
    void onResizeFrame(Event* event);
    void onResizeWindow(Event* event);
    void onRightMouseClick(Event* event);
    void onRightMouseHold(Event* event);
    void onLeftMouseClick(Event* event);
    void onLeftMouseHold(Event* event);
    void onLeftMouseRelease(Event* event);
    void onScroll(Event* event);

};

#endif