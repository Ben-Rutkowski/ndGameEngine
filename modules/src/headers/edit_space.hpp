/*
    Vertex:
    | 4f position | 4f normal | 4f center | 2f center | f select |

    Point:
    | 4f position | float select |
*/

#ifndef EDIT_SPACE_HPP
#define EDIT_SPACE_HPP

#include "edit_space_lib.hpp"
#include "rendering/window_frame.hpp"
#include "nd_module.hpp"
#include "debug_box.hpp"

#define EDIT_SPACE_SHADER_SUB_DIR "modules/src/shaders/edit_mesh/"
#define GRIDLINE_SHADER_SUB_DIR "modules/src/shaders/gridline/"
#define SELECT_BOX_SHADER_SUB_DIR "modules/src/shaders/select_box/"

/* CLASS: EditSpace */

enum esState { es_null = 0,
    /* FUNCTIONS */ esCARRY, 
    /* MODES */     esPOINT, esEDGE, esFACE,
    /* KEYS */      esLEFT_MOUSE, esSHIFT, 
    esLEN
};

class EditSpace : public ndModuleInstance<EditSpace, esLEN> {
private:
    ShaderProgram point_shader;
    ShaderProgram line_shader;
    ShaderProgram face_shader;
    ShaderProgram gridline_shader;
    ShaderProgram gridline_plane_shader;
    ShaderProgram select_box_line_shader;
    ShaderProgram select_box_face_shader;

    // WINDOW FRAME
    wFrameCDS window_frame;
    // TEST
    ShaderProgram test_window_frame_shader;

    // FACEMODE
    ShaderProgram fm_point_shader;
    ShaderProgram fm_line_shader;
    ShaderProgram fm_face_shader;


// --- Layers ---
private:
    EditCamera camera;
    GridLine x_line;
    GridLine y_line;
    GridLine z_line;

// --- Attributes ---
private:
    std::vector<EditMesh> meshes;
    SelectBox select_box;
    int selected_mesh;

public:
    EditSpace();

// --- Editing ---
private:
    Id createMesh();
    Id createDefaultCube();
    EditMesh& ref(Id);

    void translateSelectedPoints();
    void translateSelectedPoints(vec4 axis);

    void selectPointsBox();
    void selectPointsBoxAdd();
    void selectFacesClick(vec2 click);
    void selectFacesClickAdd(vec2 click);

// --- Rendering ---
private:
    void load();
    void draw();
    void drawFaceMode();

// --- Event Interface ---
public:
    void runEvent(Event* event);

private:
    void setCallbacks();

// --- On Events ---
private:
    void onDebug(Event* event);
    void onBeginLoop(Event* event);
    void onStartFrame(Event* event);
    void onDrawWindowFrame(Event* event);
    void onDraw(Event* event);
    void onEndFrame(Event* event);
    void onResizeFrame(Event* event);
    void onResizeWindow(Event* event);
    void onShiftPress(Event* event);
    void onShiftRelease(Event* event);
    void onRightMouseClick(Event* event);
    void onRightMouseHold(Event* event);
    void onLeftMouseClick(Event* event);
    void onLeftMouseHold(Event* event);
    void onLeftMouseRelease(Event* event);
    void onScroll(Event* event);
    void onWKey(Event* event);
    void onSKey(Event* event);
    void onGKeyPress(Event* event);
    void onGKeyRelease(Event* event);
};

#endif