#ifndef EDIT_SPACE_HPP
#define EDIT_SPACE_HPP

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
    ndRoutine routine;

    EditMesh test_mesh;

    SelectBuffer<EditMesh_Point_T> point_select_buffer;

    EditMesh_Point_T* points;
    EditMesh_Point_T* select;

// --- Initialization ---
public:
    ndEditSapceModule();
    ~ndEditSapceModule();
    void linkRenderer(ndRoutine routine_in);

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
