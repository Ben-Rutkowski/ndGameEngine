#include "edit_mesh/edit_mesh.hpp"
#include "math/select_algs.hpp"

void EditMesh::selectPointsBox(vec4 v1, vec4 v2, vec4 v3, vec4 camera_pos) {
    float TOL = 0.01f;

    v1 = inverse_model_pos*v1;
    v2 = inverse_model_pos*v2;
    v3 = inverse_model_pos*v3;
    camera_pos = inverse_model_pos*camera_pos;
    vec4 u = camera_pos;
    vec4 d;
    vec4 pos;
    float dist;

    int N_points = point_cache.dataLen();
    for (int i=0; i<N_points; i++) {
        if (isPointSelect(i)) { continue; }

        pos  = point(i).getPos();
        d    = vec4::subtrK(pos, camera_pos, 3);
        dist = sAlg::parRayIntersect(v1, v2, v3, u, d);
        if ( dist > TOL && !checkCullPoint(i, camera_pos) ) {
            selected_points.add(i, 0);
            setSelectPoint(i, true);
        }
    }

    // if ( selected_points.size() != 0 ) {
    //     std::cout << "Selected Points: ";
    //     for (int i=0; i<selected_points.size(); i++) {
    //         std::cout << selected_points[i] << " ";
    //     }
    //     std::cout << std::endl;
    // }
}

void EditMesh::selectFaceClick(vec4 click, vec4 camera_pos) {
    float TOL = 0.01f;

    click  = inverse_model_pos*click;
    vec4 u = inverse_model_pos*camera_pos;
    vec4 d = vec4::subtrK(click, u, 3);
    Id closest_face_id;
    float closest_dist = -1.0f;
    float curr_dist;

    int N_faces = face_cache.dataLen();
    for (int i=0; i<N_faces; i++) {
        curr_dist = face(i).rayIntersect(u, d, tri_cache, vertex_cache);
        if ( curr_dist > TOL ) {
            if ( closest_dist == -1.0f || curr_dist < closest_dist ) {
                closest_dist = curr_dist;
                closest_face_id = i;
            }
        }
    }

    if ( closest_dist != -1.0f ) {
        selected_faces.add(closest_face_id, 0);
        setSelectFace(closest_face_id, true);
        // std::cout << "Selected Face: " << closest_face_id << std::endl;
        // face(closest_face_id).printSmall();
    }
}

void EditMesh::clearSelectedPoints() {
    int N_select = selected_points.size();
    for (int i=0; i<N_select; i++) {
        setSelectPoint(selected_points[i], false);
    }
    selected_points.clear();
}

void EditMesh::clearSelectedFaces() {
    int N_select = selected_faces.size();
    for (int i=0; i<N_select; i++) {
        setSelectFace(selected_faces[i], false);
    }
    selected_faces.clear();
}

Id EditMesh::findEdge(Id point_id_0, Id point_id_1) {
    Id cur_edge_id;
    Id other_point_id;
    int N_edges = point_cache.pairedEdgeLen(point_id_0);
    for (int i=0; i<N_edges; i++) {
        cur_edge_id = pointToEdge(point_id_0, i);
        other_point_id = edge(cur_edge_id).otherId(point_id_0);
        if (other_point_id == point_id_1) {
            return cur_edge_id;
        }
    }
    return 0;
}

// === Private ===
void EditMesh::setSelectPoint(Id point_id, bool value) {
    point(point_id).setSelect(value);
    reloadPoint(point_id);
}

void EditMesh::setSelectFace(Id face_id, bool value) {
    Id vert_id;
    int N_verts = face(face_id).pointLen();
    for (int i=0; i<N_verts; i++) {
        vert_id = face(face_id).vertId(i);

        vertex(vert_id).setSelect(value);
        reloadVertex(vert_id);
    }
}

void EditMesh::collectAffectedEdges(IdSet& point_ids, IdSet& emtpy_edge_ids) {
    int max = 0;
    int N_points = point_ids.size();
    for (int i=0; i<N_points; i++) {
        max += point_cache.pairedEdgeLen(point_ids[i]);
    }
    max = std::min(max, edge_cache.dataLen());

    Id cur_edge_id;
    emtpy_edge_ids.resizeClear(max);
    int M_edges;
    for (int i=0; i<N_points; i++) {
        M_edges = point_cache.pairedEdgeLen(point_ids[i]);
        for (int j=0; j<M_edges; j++) {
            cur_edge_id = pointToEdge(point_ids[i], j);
            emtpy_edge_ids.add(cur_edge_id);
        }
    }
}

void EditMesh::collectAffectedFaces(IdSet& point_ids, IdSet& emtpy_face_ids) {
    int max = 0;
    int N_points = point_ids.size();
    for (int i=0; i<N_points; i++) {
        max += point_cache.pairedFaceLen(point_ids[i]);
    }
    max = std::min(max, face_cache.dataLen());

    Id cur_face_id;
    emtpy_face_ids.resizeClear(max);
    int M_faces;
    for (int i=0; i<N_points; i++) {
        M_faces = point_cache.pairedFaceLen(point_ids[i]);
        for (int j=0; j<M_faces; j++) {
            cur_face_id = pointToFace(point_ids[i], j);
            emtpy_face_ids.add(cur_face_id);
        }
    }
}

bool EditMesh::isPointSelect(Id point_id) {
    return selected_points.hasElement(point_id);
}

bool EditMesh::checkCullPoint(Id point_id, vec4 camera_pos) {
    float TOL = 0.01f;

    vec4 u = point(point_id).getPos();
    vec4 d = vec4::subtrK(camera_pos, u, 3);
    float dist;

    int N_faces = face_cache.dataLen();
    for (int i=0; i<N_faces; i++) {
        dist = face(i).rayIntersect(u, d, tri_cache, vertex_cache);
        if ( dist > TOL ) {
            return true;
        }
    }
    return false;
}