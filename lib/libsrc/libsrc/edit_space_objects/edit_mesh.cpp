// #include "math/matrix.hpp"
#include "edit_space_objects/edit_mesh.hpp"
// #include <iostream>

// ================ Initialization ================
EditMesh::EditMesh() {
    initializeWithDefaultCube();
}

void EditMesh::initializeWithDefaultCube() {

}


// ================ Creating DCEL Objects ================
uint32_t EditMesh::createQuad(const std::array<uint32_t,4> new_points) {
    uint32_t start_id = half_edges_dcel.size();
    half_edges_dcel.insert(half_edges_dcel.end(), 4, DCEL_HalfEdge());

    for (int i=0; i<4; i++) {
        half_edges_dcel[start_id+i].vertex = new_points[i];
        half_edges_dcel[start_id+i].next_hedge = start_id + (i+1)%4;
        half_edges_dcel[start_id+i].prev_hedge = start_id + (i+3)%4;
        half_edges_dcel[start_id+i].twin_hedge = start_id + 4;
    }

    return 0;
}
