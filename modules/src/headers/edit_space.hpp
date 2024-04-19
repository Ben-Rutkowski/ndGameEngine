#ifndef EDIT_SPACE_HPP
#define EDIT_SPACE_HPP

#include "nd_module.hpp"
#include "edit_space_objects/edit_mesh.hpp"
#include "shader_types/edit_mesh_types.h"

#include <vector>

typedef enum ndEditSpaceState { ndEditSpaceStateNull = 0,
    ndEditSpaceStateLen    
} ndEditSpaceState;

typedef struct EditMesh_PointCluster {
    EditMesh_Point_T point;
    uint32_t aux_data;
} EditMesh_PointCluster;


class ndEditSapceModule : public ndModuleImplement<ndEditSpaceStateLen> {
private:
    EditMesh test_mesh;

    // --- DEBUG ---
    std::vector<EditMesh_PointCluster> point_buffer;
    std::vector<EditMesh_PointCluster> select_point_buffer;

// --- Initialization ---
public:
    ndEditSapceModule();

// --- Buffer Operations ---
public:
    uint32_t addPoint(const vec4 point);
    uint32_t deletePoint(const uint32_t d_point);
    void selectPoint(const uint32_t s_point);
    void flushSelectedPoints();

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
