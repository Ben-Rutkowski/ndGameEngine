#include "edit_space_objects/edit_mesh.hpp"

EditMesh::EditMesh() {}

void EditMesh::initializeWithDefaultCube() {
    // --- initialize vertex stroage ---
    point_vertices = std::vector<V_EditMesh_pt>(8);
    tri_vertices   = std::vector<V_EditMesh_tri>(8);

    // --- initialize index storage ---
    edge_indices = std::vector<unsigned int>(24);
    tri_indices  = std::vector<unsigned int>(36);

    // --- initialize mesh storage ---
    half_edges = std::vector<DCEL_HalfEdge>(24);
    faces      = std::vector<DCEL_Face>(8);

    // --- Create Point Vertices ---
    // point_vertices[0].position = { -0.5f, -0.5f, -0.5f, 1.0f };
    // point_vertices[1].position = {  0.5f, -0.5f, -0.5f, 1.0f };
    // point_vertices[2].position = {  0.5f, -0.5f,  0.5f, 1.0f };
    // point_vertices[3].position = { -0.5f, -0.5f,  0.5f, 1.0f };
    // point_vertices[4].position = { -0.5f,  0.5f, -0.5f, 1.0f };
    // point_vertices[5].position = {  0.5f,  0.5f, -0.5f, 1.0f };
    // point_vertices[6].position = {  0.5f,  0.5f,  0.5f, 1.0f };
    // point_vertices[7].position = { -0.5f,  0.5f,  0.5f, 1.0f };

	// point_vertices[0].color = { 0.4f, 0.4f, 0.4f, 1.0f };
	// point_vertices[1].color = { 0.4f, 0.4f, 0.4f, 1.0f };
	// point_vertices[2].color = { 0.4f, 0.4f, 0.4f, 1.0f };
	// point_vertices[3].color = { 0.4f, 0.4f, 0.4f, 1.0f };
	// point_vertices[4].color = { 0.4f, 0.4f, 0.4f, 1.0f };
	// point_vertices[5].color = { 0.4f, 0.4f, 0.4f, 1.0f };
	// point_vertices[6].color = { 0.4f, 0.4f, 0.4f, 1.0f };
	// point_vertices[7].color = { 0.4f, 0.4f, 0.4f, 1.0f };

    // point_vertices[0].uv = { 0.0f, 0.0f };
    // point_vertices[1].uv = { 0.0f, 0.0f };
    // point_vertices[2].uv = { 0.0f, 0.0f };
    // point_vertices[3].uv = { 0.0f, 0.0f };
    // point_vertices[4].uv = { 0.0f, 0.0f };
    // point_vertices[5].uv = { 0.0f, 0.0f };
    // point_vertices[6].uv = { 0.0f, 0.0f };
    // point_vertices[7].uv = { 0.0f, 0.0f };
}
