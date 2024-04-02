#ifndef EDIT_SPACE_HPP
#define EDIT_SPACE_HPP

#include "nd_module.hpp"
#include "edit_space_objects/edit_mesh.hpp"
#include "shader_types/edit_mesh_types.h"

#include <vector>

typedef enum ndEditSpaceState { ndEditSpaceStateNull = 0,
    ndEditSpaceStateLen    
} ndEditSpaceState;


class ndEditSapceModule : public ndModuleImplement<ndEditSpaceStateLen> {
private:
    EditMesh test_mesh;

    // --- DEBUG ---
    std::vector<EditMesh_Point_T> point_buffer;


// --- Initialization ---
public:
    ndEditSapceModule();

// --- Buffer Operations ---
public:
    uint32_t addPoints(const vec4* points, const int len);
    uint32_t deletePoint(const uint32_t d_point);

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
