#include "edit_space.hpp"
#include "shader_types/edit_mesh_types.h"

ndEditSapceModule::ndEditSapceModule()
:ndModuleImplement<ndEditSpaceStateLen>(Module::EDIT_SPACE)
{
    point_buffer.reserve(16);
    select_point_buffer.reserve(16);
}


// ================ Buffer Operations ================
uint32_t ndEditSapceModule::addPoint(const vec4 point) {
    EditMesh_PointCluster a_point;
    a_point.point.position = point;
    a_point.aux_data = 1;

    uint32_t start_idx = point_buffer.size();
    point_buffer.push_back(a_point);
    return start_idx;
}

uint32_t ndEditSapceModule::deletePoint(const uint32_t d_point) {
    point_buffer[d_point] = point_buffer.back();
    point_buffer.pop_back();
    return point_buffer.size();
}

void ndEditSapceModule::selectPoint(const uint32_t s_point) {
    select_point_buffer.push_back(point_buffer[s_point]);
    select_point_buffer.back().aux_data = s_point;
    point_buffer[s_point].aux_data = 0;
}

void ndEditSapceModule::flushSelectedPoints() {
    EditMesh_PointCluster cur_cluster;
    uint32_t s_idx;
    int select_size = select_point_buffer.size();

    for (int i=0; i<select_size; i++) {
        cur_cluster = select_point_buffer.back();
        select_point_buffer.pop_back();

        s_idx = cur_cluster.aux_data;
        point_buffer[s_idx].point = cur_cluster.point;
        point_buffer[s_idx].aux_data = 1;
    }
}


// ================ Debugging ================
void ndEditSapceModule::debug() {
    std::cout << "Debugging: <ndEditSpaceModule>" << std::endl;
    std::cout << "Point Buffer: " << std::endl;
    for (int i=0; i<point_buffer.size(); i++) {
        std::cout << i << ": (";
        std::cout << point_buffer[i].aux_data << ") ";
        std::cout << "position - ";
        point_buffer[i].point.position.debug();
    }

    std::cout << "Selected Buffer: " << std::endl;
    for (int i=0; i<select_point_buffer.size(); i++) {
        std::cout << i << ": (";
        std::cout << select_point_buffer[i].aux_data << ") ";
        std::cout << "position - ";
        select_point_buffer[i].point.position.debug();
    }

    std::cout << std::endl;
}
