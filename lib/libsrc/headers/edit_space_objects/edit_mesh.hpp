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
// --- Mesh Data ---
    std::vector<DCEL_HalfEdge> half_edges_dcel;
    std::vector<DCEL_Face>     faces_dcel;

// --- Initialization ---
public:
    EditMesh();

    void initializeWithDefaultCube();

private:
    // --- Creating DCEL Objects ---
    uint32_t createQuad(const std::array<uint32_t,4> new_points);

// --- Debugging ---
public:
    void debug();
};
#endif
