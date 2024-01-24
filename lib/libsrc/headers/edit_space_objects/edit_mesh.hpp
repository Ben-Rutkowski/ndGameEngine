#ifndef EDIT_MESH_HPP
#define EDIT_MESH_HPP

#include "math/matrix.hpp"
#include "shader_types.h"
#include <vector>

struct DCEL_HalfEdge {
    int vertex;
    int twin_hedge;
    int next_hedge;
    int prev_hedge;
};

struct DCEL_Face {
    int inner_hedge;
};

class EditMesh {
private:
// --- Render Data ---
    std::vector<V_EditMesh_pt>  point_vertices; 
    std::vector<V_EditMesh_tri> tri_vertices;
    std::vector<unsigned int>   edge_indices;
    std::vector<unsigned int>   tri_indices;

// --- Mesh Data ---
    std::vector<DCEL_HalfEdge> half_edges;
    std::vector<DCEL_Face>     faces;

// --- Initialization ---
public:
    EditMesh();

    void initializeWithDefaultCube();

private:
    void createNewTriVertices(int face_index);
};

#endif