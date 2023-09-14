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

void EditMesh::selectPoint(Id point_id, bool value) {
    select_points[point_id] = value;

    point(point_id).setSelect(value);
    reloadPoint(point_id);
}

void EditMesh::selectFace(Id face_id, bool value) {
    select_faces[face_id] = value;

    int N_verts = face_cache[face_id].vertLen();
    for (int i=0; i<N_verts; i++) {
        face(face_id).vert(i, vertex_cache).setSelect(value);
    }

    reloadFace(face_id);
}

void EditMesh::cullPoint(Id id, vec4 camera_pos) {
    float TOL = 0.01f;

    float dist;
    vec4 u = point_cache[id].getPos();
    vec4 d = vec4::subtrK(camera_pos, u, 3);
    int N = face_cache.dataLen();
    for (int i=0; i<N; i++) {
        dist = face_cache[i].rayIntersect(u, d, tri_cache, vertex_cache);
        if ( dist > TOL ) {
            selectPoint(id, false);
            return;
        }
    }
}