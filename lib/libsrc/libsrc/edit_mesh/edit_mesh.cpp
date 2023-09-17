#include "edit_mesh/edit_mesh.hpp"
#include "math/select_algs.hpp"

EditMesh::EditMesh()
    :model_pos{ mat4::iden() },
    inverse_model_pos{ mat4::iden() } {} 

mat4 EditMesh::getModel() {
    return model_pos;
}

EditPoint&    EditMesh::point(Id point_id) { return point_cache[point_id]; }
EditVertex&   EditMesh::vertex(Id vert_id) { return vertex_cache[vert_id]; }
EdgeIndexObj& EditMesh::edge(Id edge_id)   { return edge_cache[edge_id]; }
TriIndexObj&  EditMesh::tri(Id tri_id)     { return tri_cache[tri_id]; }
EditFace&     EditMesh::face(Id face_id)   { return face_cache[face_id]; }

Id EditMesh::vertToPoint(Id vert_id) 
    { return vertex_cache.getPairedPoint(vert_id); }
Id EditMesh::pointToVert(Id point_id, int i) 
    { return point_cache.getPairedVertex(point_id, i); }
Id EditMesh::pointToEdge(Id point_id, int i)
    { return point_cache.getPairedEdge(point_id, i); }
Id EditMesh::pointToFace(Id point_id, int i)
    { return point_cache.getPairedFace(point_id, i); }
Id EditMesh::edgeToFace(Id edge_id, int i)
    { return edge_cache.getPairedFace(edge_id, i); }

void EditMesh::translate(vec4 trans) {
    model_pos = mat4::translate(trans)*model_pos;
    mat4 inverse = mat4::itranslate(trans);
    inverse_model_pos = inverse_model_pos*inverse;
}

// === Debugging ===
vec4 EditMesh::getPoint(Id id) {
    return point_cache[id].getPos();
}

void EditMesh::printSelect() {
    int N_select = selected_points.capacity();
    for (int i=0; i<N_select; i++) {
        std::cout << selected_points[i] << " ";
    }
    std::cout << std::endl;
}

void EditMesh::debug() {
    IdSet faces(1); faces.add(0);
    ripPointTest(5, faces);
    // load();
}