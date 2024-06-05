#include "edit_space.hpp"
#include "shader_types/edit_mesh_types.h"

ndEditSapceModule::ndEditSapceModule()
:ndModuleImplement<ndEditSpaceStateLen>(Module::EDIT_SPACE),
 camera(0.1f, 100.0f, 45.0f, 800.0f/600.0f)
{
    points = new EditMesh_Point_T[16];
    select = new EditMesh_Point_T[16];
    point_select_buffer.linkBuffers(points, 16, select, 16);
}

ndEditSapceModule::~ndEditSapceModule() {
    delete[] points;
    delete[] select;
}

uint32_t ndEditSapceModule::createDefualtCubePoints(uint32_t start) {
    points[start].position   = { -0.5f, -0.5f, 0.0f, 1.0f };
    points[start+1].position = { -0.5f, -0.5f, 0.0f, 1.0f };
    points[start+2].position = { -0.5f, -0.5f, 0.0f, 1.0f };
    points[start+3].position = { -0.5f, -0.5f, 0.0f, 1.0f };

    return 0;
}
