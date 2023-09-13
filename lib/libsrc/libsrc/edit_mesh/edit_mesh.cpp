#include "edit_mesh/edit_mesh.hpp"
#include "math/select_algs.hpp"


EditMesh::EditMesh()
    :model_pos{ mat4::iden() },
    inverse_model_pos{ mat4::iden() } {} 

mat4 EditMesh::getModel() {
    return model_pos;
}

void EditMesh::translate(vec4 trans) {
    model_pos = mat4::translate(trans)*model_pos;
    mat4 inverse = mat4::itranslate(trans);
    inverse_model_pos = inverse_model_pos*inverse;
}

void EditMesh::reloadPoint(Id point) {
    point_vbi.bindAllBuffers();
    point_vbi.editVertexData(&point_cache[point], sizeof(EditPoint), point*sizeof(EditPoint));
    point_vbi.unbindCurrent();
    line_vbi.bindAllBuffers();
    line_vbi.editVertexData(&point_cache[point], sizeof(EditPoint), point*sizeof(EditPoint));
    line_vbi.unbindCurrent();
}

void EditMesh::reloadFace(Id face) {
    Id vert_id;
    int N = face_cache[face].vertLen();
    face_vbi.bindAllBuffers();
    for (int i=0; i<N; i++) {
        vert_id = face_cache[face].vertId(i);
        face_vbi.editVertexData(&vertex_cache[vert_id], sizeof(EditVertex), vert_id*sizeof(EditVertex));
    }
    face_vbi.unbindCurrent();
}

// === Debugging ===
vec4 EditMesh::getPoint(Id id) {
    return point_cache[id].getPos();
}

void EditMesh::printSelect() {
    int N = point_cache.dataLen();
    for (int i=0; i<N; i++) {
        if (select_points[i]) {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
}