#ifndef EDIT_MESH_HPP
#define EDIT_MESH_HPP

#include "math/matrix.hpp"
#include "shader_types/shader_types.h"
#include <vector>

struct DCEL_HalfEdge {
    unsigned int vertex;
    unsigned int next_hedge;
    unsigned int prev_hedge;
    unsigned int twin_hedge;
};

struct DCEL_Face {
    unsigned int inner_hedge;
    unsigned int size;
};

class EditMesh {
private:
// --- Render Data ---
    std::vector<V_EditMesh_pt> point_vertices;
    std::vector<unsigned int>  edge_indices;
    std::vector<unsigned int>  tri_indices;

// --- Mesh Data ---
    std::vector<DCEL_HalfEdge> half_edges_dcel;
    std::vector<DCEL_Face>     faces_dcel;

// --- Initialization ---
public:
    EditMesh();

    void initializeWithDefaultCube();

// --- Creating Objects ---
private:
    unsigned int addNewFace(const std::array<unsigned int,4> points);

// --- Editing Objects ---
private:
    void stitchFaces(unsigned int face_0,
                     unsigned int face_1,
                     unsigned int N_e0,
                     unsigned int N_e1);

// --- Debugging ---
public:
    void debug();
};
#endif
