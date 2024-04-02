#include "math/matrix.hpp"
#include "edit_space_objects/edit_mesh.hpp"
#include <iostream>

// ================ Initialization ================
EditMesh::EditMesh() {
    initializeWithDefaultCube();
}

void EditMesh::initializeWithDefaultCube() {

}


// ================ Creating DCEL Objects ================
uint32_t EditMesh::createQuad(const std::array<uint32_t,4> points) {
    // --- Create DCEL Objects ---
    uint32_t hedge_start = half_edges_dcel.size();
    half_edges_dcel.insert(half_edges_dcel.end(), 8, DCEL_HalfEdge());

    for (int i=0; i<4; i++) {
        half_edges_dcel[hedge_start+i].vertex     = points[i];
        half_edges_dcel[hedge_start+i].next_hedge = hedge_start + (i+1)%4;
        half_edges_dcel[hedge_start+i].prev_hedge = hedge_start + (i+3)%4;
        half_edges_dcel[hedge_start+i].twin_hedge = hedge_start + i+4;

        half_edges_dcel[hedge_start+i+4].vertex     = points[(i+1)%4];
        half_edges_dcel[hedge_start+i+4].next_hedge = 0;
        half_edges_dcel[hedge_start+i+4].prev_hedge = 0;
        half_edges_dcel[hedge_start+i+4].twin_hedge = hedge_start + i;
    }

    uint32_t new_face = faces_dcel.size();
    faces_dcel.push_back(DCEL_Face());
    faces_dcel[new_face].inner_hedge = hedge_start;
    faces_dcel[new_face].size = 4;

    return new_face;
}
