#ifndef EDIT_SPACE_HPP
#define EDIT_SPACE_HPP

#include "edit_space/edit_mesh.hpp"
#include "edit_space/select_box.hpp"
#include "edit_space/edit_camera.hpp"
#include "edit_space/gridline.hpp"
#include "nd_module.hpp"

#define EDIT_SPACE_SHADER_SUB_DIR "modules/src/shaders/edit_mesh/"
#define GRIDLINE_SHADER_SUB_DIR "modules/src/shaders/gridline/"
#define SELECT_BOX_SHADER_SUB_DIR "modules/src/shaders/select_box/"

/* CLASS: EditSpace
*/

enum esState { es_null = 0,
    /* KEYS */ esLEFT_MOUSE,
    esLEN
};

class EditSpace : public ndModule {
private:
    ShaderProgram point_shader;
    ShaderProgram line_shader;
    ShaderProgram face_shader;
    ShaderProgram gridline_shader;
    ShaderProgram gridline_plane_shader;
    ShaderProgram select_box_line_shader;
    ShaderProgram select_box_face_shader;

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
    void onWKey(Event* event);
    void onSKey(Event* event);
};

#endif