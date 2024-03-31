#ifndef EDIT_MESH_HPP
#define EDIT_MESH_HPP

#include <stdint.h>
#include <vector>

struct DCEL_HalfEdge {
    uint32_t vertex;
    uint32_t next_hedge;
    uint32_t prev_hedge;
    uint32_t twin_hedge;
};

struct DCEL_Face {
    uint32_t inner_hedge;
    uint32_t size;
};

class EditMesh {
private:
// --- Render Data ---
    // std::vector<V_EditMesh_pt> point_vertices;
    // std::vector<unsigned int>  edge_indices;
    // std::vector<unsigned int>  tri_indices;

// --- Mesh Data ---
    // std::vector<DCEL_HalfEdge> half_edges_dcel;
    // std::vector<DCEL_Face>     faces_dcel;

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
