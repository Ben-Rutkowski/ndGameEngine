#include "edit_space.hpp"
#include "shader_types/edit_mesh_types.h"

ndEditSapceModule::ndEditSapceModule()
:ndModuleImplement<ndEditSpaceStateLen>(Module::EDIT_SPACE)
{

}


// ================ Buffer Operations ================
uint32_t ndEditSapceModule::addPoints(const vec4* points, const int len) {
    uint32_t start_idx = point_buffer.size();
    point_buffer.insert(point_buffer.end(), len, EditMesh_Point_T());

    for (int i=0; i<len; i++) {
        point_buffer[start_idx+i].position = points[i];
    }

    return start_idx;
}


uint32_t ndEditSapceModule::deletePoint(const uint32_t d_point) {
    point_buffer[d_point] = point_buffer.back();
    point_buffer.pop_back();
    return point_buffer.size();
}


// ================ Debugging ================
void ndEditSapceModule::debug() {
    std::cout << "Debugging: <ndEditSpaceModule>" << std::endl;
    std::cout << "Point Buffer: " << std::endl;
    for (int i=0; i<point_buffer.size(); i++) {
        std::cout << i << ": ";
        std::cout << "position - ";
        point_buffer[i].position.debug();
    }
}
