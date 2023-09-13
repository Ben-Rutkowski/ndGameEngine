#include "edit_mesh/edit_mesh.hpp"
#include "math/select_algs.hpp"

void EditMesh::selectFacesClick(vec4 point, vec4 camera_pos) {
    point = inverse_model_pos*point;
    vec4 u = inverse_model_pos*camera_pos;
    vec4 d = vec4::subtrK(point, u, 3);

    float curr_dist;
    float least_dist = -1.0f;
    int selected_face = -1;
    int N = face_cache.dataLen();
    for (int i=0; i<N; i++) {
        curr_dist = face_cache[i].rayIntersect(u, d, tri_cache, vertex_cache);
        if (curr_dist != -1.0f) {
            if (least_dist == -1.0f) {
                least_dist = curr_dist;
                selected_face = i;
            } else if ( curr_dist < least_dist ) {
                least_dist = curr_dist;
                selected_face = i;
            }
        }
    }
    
    bool select;
    for (int i=0; i<N; i++) {
        select = i == selected_face;
        selectFace(i, select);
    }
}

void EditMesh::selectPointsBox(vec4 v1, vec4 v2, vec4 v3, vec4 camera_pos) {
    v1 = inverse_model_pos*v1;
    v2 = inverse_model_pos*v2;
    v3 = inverse_model_pos*v3;
    camera_pos = inverse_model_pos*camera_pos;
    vec4 u = camera_pos;
    vec4 d;
    vec4 point;
    float dist;
    bool selected;

    int N = point_cache.dataLen();
    for (int i=0; i<N; i++) {
        point = point_cache[i].getPos();
        d     = vec4::subtrK(point, camera_pos, 3);
        dist  = sAlg::parRayIntersect(v1, v2, v3, u, d);
        selected = dist > 0.0f;
        selectPoint(i, selected);
        if ( selected ) {
            cullPoint(i, camera_pos);
        }
    }
}

// === Private ===
void EditMesh::resetSelectPoints() {
    int N = point_cache.dataLen();
    select_points.assign(N, false);
}

void EditMesh::resetSelectFaces() {
    int N = face_cache.dataLen();
    select_faces.assign(N, false);
}

void EditMesh::selectPoint(Id id, bool value) {
    select_points[id] = value;
    point_cache[id].setSelect(value);
    updatePoint(id);
}

void EditMesh::selectFace(Id id, bool value) {
    select_faces[id] = value;
    int N = face_cache[id].vertLen();
    Id vert;
    for (int i=0; i<N; i++) {
        vert = face_cache[id].vertId(i);
        vertex_cache[vert].setSelect(value);
        updateVertex(vert);
    }
}

void EditMesh::cullPoint(Id id, vec4 camera_pos) {
    float dist;
    vec4 u = point_cache[id].getPos();
    vec4 d = vec4::subtrK(camera_pos, u, 3);
    int N = face_cache.dataLen();
    for (int i=0; i<N; i++) {
        dist = face_cache[i].rayIntersect(u, d, tri_cache, vertex_cache);
        if ( dist != -1.0f) {
            selectPoint(id, false);
            return;
        }
    }
}