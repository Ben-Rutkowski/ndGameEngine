#ifndef EDIT_SPACE_HPP
#define EDIT_SPACE_HPP

#include "camera.hpp"
#include "nd_module.hpp"
#include "edit_space_objects/edit_mesh.hpp"
#include "shader_types/edit_mesh_types.h"
#include "select_buffer.hpp"
#include "cocoa_interface.hpp"

typedef enum ndEditSpaceState { ndEditSpaceStateNull = 0,
    ndEditSpaceStateLen    
} ndEditSpaceState;


class ndEditSapceModule : public ndModuleImplement<ndEditSpaceStateLen> {
private:
// --- Rendering ---
    ndRoutine routine;
    Camera    camera;

// --- Selection ---
    SelectBuffer<EditMesh_Point_T> point_select_buffer;

// --- Debugging ---
    EditMesh test_mesh;
    EditMesh_Point_T* points;
    EditMesh_Point_T* select;

// --- Initialization ---
public:
    ndEditSapceModule();
    ~ndEditSapceModule();

// --- Creating Points ---
public:
    uint32_t createDefualtCubePoints(uint32_t start);

// --- Events ---
public:
    void runEvent(ndEvent* event);

public:
    void onDebug(ndEvent* event);
    void onBeginStartUp(ndEvent* event);
    void onEndStartUp(ndEvent* event);
    void onStartFrame(ndEvent* event);
    void onEndFrame(ndEvent* event);

    void onDrawWindowFrame(ndEvent* event);
    void onDraw(ndEvent* event);

    void onResizeFrame(ndEvent* event);

// --- Debugging ---
public:
    void debug();
};

#endif
