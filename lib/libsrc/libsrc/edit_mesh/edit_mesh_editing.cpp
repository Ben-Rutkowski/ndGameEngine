#include "edit_mesh/edit_mesh.hpp"

Id EditMesh::createPoint(vec4 pos) {
    return point_cache.addPoint(pos);
}

Id EditMesh::createEdge(Id2 point_ids) {
    Id new_edge = edge_cache.addEdge(EdgeIndexObj(point_ids[0], point_ids[1]));
    point_cache.pairEdge(point_ids[0], new_edge);
    point_cache.pairEdge(point_ids[1], new_edge);
    return new_edge;
}

Id EditMesh::createTri(Id3 point_ids, Id3 edges_id) {
    Id3 vert_ids;
    Id  tri_id;
    Id  face_id = face_cache.createFace(3);

    vec4 curr_point_pos;
    for (int i=0; i<3; i++) {
        // Pair Point and Edge
        point_cache.pairFace(point_ids[i], face_id);
        edge_cache.pairFace(edges_id[i], face_id);

        // Create Vertex
        curr_point_pos = point(point_ids[i]).getPos();
        vert_ids[i] = vertex_cache.addVertex(curr_point_pos);

        // Add to Face
        face(face_id).addPoint(point_ids[i], vert_ids[i]);
        face(face_id).addEdge(edges_id[i]);
    }
    tri_id = tri_cache.addTri(vert_ids);

    // Add to Face
    face(face_id).addTri(tri_id);

    recalculateFaceCenter(face_id);
    recalculateFaceNormal(face_id);

    return face_id;
}

Id EditMesh::createQuad(Id4 point_ids, Id4 edge_ids) {
    Id4 vert_ids;
    Id2 tri_ids;
    Id  face_id = face_cache.createFace(4);

    vec4 curr_point_pos;
    for (int i=0; i<4; i++) {
        // Pair Point and Edge
        point_cache.pairFace(point_ids[i], face_id);
        edge_cache.pairFace(edge_ids[i], face_id);

        // Create Vertex
        curr_point_pos = point(point_ids[i]).getPos();
        vert_ids[i] = vertex_cache.addVertex(curr_point_pos);

        // Add to Face
        face(face_id).addPoint(point_ids[i], vert_ids[i]);
        face(face_id).addEdge(edge_ids[i]);
    }
    tri_ids[0] = tri_cache.addTri(vert_ids[0], vert_ids[1], vert_ids[2]);
    tri_ids[1] = tri_cache.addTri(vert_ids[2], vert_ids[3], vert_ids[0]);

    // Add to Face
    face(face_id).addTri(tri_ids[0]);
    face(face_id).addTri(tri_ids[1]);

    recalculateFaceCenter(face_id);
    recalculateFaceNormal(face_id);

    return face_id;
}

Id EditMesh::createQuadFromEdge(Id point_id_0, Id point_id_1, IdSet& point_id_pairs, IdSet& edge_id_pairs) {
    Id point_id_2   = point_id_pairs.value(point_id_1);
    Id point_id_3   = point_id_pairs.value(point_id_0);
    Id edge_id_root = findEdge(point_id_0, point_id_1);
    Id edge_id_op   = edge_id_pairs.value(edge_id_root);
    Id edge_side_0  = createEdge({point_id_0, point_id_3});
    Id edge_side_1  = createEdge({point_id_1, point_id_2});
    Id quad_id      = createQuad(
        {point_id_0, point_id_1, point_id_2, point_id_3},
        {edge_id_root, edge_id_op, edge_side_0, edge_side_1}
    );
    return quad_id;
}

void EditMesh::transformPoints(IdSet& point_ids, mat4 mat) {
    // Collect Affected Faces
    IdSet affected_faces;
    collectAffectedFaces(point_ids, affected_faces);

    // Move Points
    vec4 cur_pos;
    int N_points = point_ids.size();
    for (int i=0; i<N_points; i++) {
        cur_pos = point(point_ids[i]).getPos();
        cur_pos = mat*cur_pos;
        point(point_ids[i]).setPos(cur_pos);
    }

    // Recalculate Faces
    Id cur_face_id;
    int N_faces = affected_faces.size();
    for (int i=0; i<N_faces; i++) {
        cur_face_id = affected_faces[i];
        recalculateFacePos(cur_face_id, point_ids);
        recalculateFaceCenter(cur_face_id);
        recalculateFaceNormal(cur_face_id);
    }

    // Reload Point
    for (int i=0; i<N_points; i++) {
        reloadPoint(point_ids[i]);
    }

    // Reload Faces
    for (int i=0; i<N_faces; i++) {
        reloadFace(affected_faces[i]);
    }
}

void EditMesh::transformPoints(mat4 mat) {
    transformPoints(selected_points, mat);
}

void EditMesh::ripPoints(IdSet& points_attatched, IdSet& edge_pairs, IdSet& faces_attatched) {
/*
    Function rips the points off of a mesh. The old points are 
    attatched to faces_attatched. A new points are created to
    replace the old points in the remaining faces and edges.
    At the end of the function, the old_point_ids have paired
    with them the new points. Edge pairs will be resized of
    the size of the number of edge seems. It will be filled
    with keys of the old seems and paired with the new seems.

    There are three types of edges, those that belong only to 
    the attatched faces, those that only to the bottom faces,
    and those are on the seem. The seem edges are split into
    two: top and bottom, these are created.
*/
    IdSet edge_map;
    
    // Create new points.
    Id   new_point_id;
    Id   old_point_id;
    vec4 cur_pos;
    int N_points = points_attatched.size();
    for (int i=0; i<N_points; i++) {
        old_point_id = points_attatched[i];
        cur_pos      = point(old_point_id).getPos();
        new_point_id = createPoint(cur_pos);
        points_attatched.forceSetValue(new_point_id, i);
    }

    // Replace old points with new points in the fixed faces.
    for (int i=0; i<N_points; i++) {
        old_point_id = points_attatched[i];
        new_point_id = points_attatched.forceGetValue(i);
        replacePointInFaces(old_point_id, new_point_id, faces_attatched, true);
    }

    // Classify edges
    collectAffectedEdges(points_attatched, edge_map);
    int seem_num = classifyEdges(edge_map, faces_attatched);
    edge_pairs.resizeClear(seem_num);

    // Process Edges
    Id cur_edge_id;
    Id new_edge_id;
    uint edge_type;
    int N_edges = edge_map.size();
    for (int i=0; i<N_edges; i++) {
        cur_edge_id = edge_map[i];
        edge_type   = edge_map.forceGetValue(i);
        if (edge_type == emSEEM) {
            new_edge_id = duplicateEdge(cur_edge_id, points_attatched);
            replaceEdgeInFaces(cur_edge_id, new_edge_id, faces_attatched, true);
            edge_pairs.add(cur_edge_id, new_edge_id);
        } else if (edge_type == emBOTTOM) {
            replacePointsInEdge(points_attatched, cur_edge_id);
        }
    }
}

// ================ Testing ================
void EditMesh::extrudeFaceTest(Id face_id) {
    clearSelectedPoints();

    std::vector<Id> wrap_order = {4, 5, 6, 7};
    IdSet face_ids(1);
    face_ids.add(0);
    IdSet point_ids;
    IdSet edge_pairs;

    Id cur_point_id;
    int N_points = face(face_id).pointLen();
    point_ids.resizeClear(N_points);
    for (int i=0; i<N_points; i++) {
        cur_point_id = face(face_id).pointId(i);
        point_ids.add(cur_point_id);
    }

    ripPoints(point_ids, edge_pairs, face_ids);

    mat4 TRANS = mat4::translate(vec4({0.0f, 0.5f, 0.0f, 0.0f}));
    transformPoints(point_ids, TRANS);

    int j;
    for (int i=0; i<4; i++) {
        j = (i+1)%4;
        createQuadFromEdge(wrap_order[j], wrap_order[i], point_ids, edge_pairs);
    }

    load();
}

// ================ Private ================
void EditMesh::recalculateFaceCenter(Id face_id) {
    vec4 center = face(face_id).calcCenter(point_cache);
    face(face_id).setCenter(center, vertex_cache);
}

void EditMesh::recalculateFaceNormal(Id face_id) {
    vec4 normal = face(face_id).calcNorm(tri_cache, vertex_cache);
    face(face_id).setNorm(normal, vertex_cache);
}

void EditMesh::recalculateFacePos(Id face_id, IdSet& point_ids) {
    vec4 pos;   
    Id   cur_point_id;
    int N_points = face(face_id).pointLen();
    for (int i=0; i<N_points; i++) {
        cur_point_id = face(face_id).pointId(i);
        if (point_ids.hasElement(cur_point_id)) {
            pos = face(face_id).calcVertPos(cur_point_id, point_cache);
            face(face_id).setPos(cur_point_id, pos, vertex_cache);
        }
    }
}

void EditMesh::replacePointInFaces(Id old_point_id, Id new_point_id, IdSet& face_ids, bool invert) {
    std::vector<Id> affected_faces;

    // Collect affected faces
    Id   cur_face_id;
    bool in_set;
    int  N_faces = point_cache.pairedFaceLen(old_point_id);
    affected_faces.reserve(N_faces);
    for (int i=0; i<N_faces; i++) {
        cur_face_id = pointToFace(old_point_id, i);
        in_set      = face_ids.hasElement(cur_face_id);
        if ( in_set == !invert ) {
            affected_faces.push_back(cur_face_id);
        }
    }

    // Replace points in affected faces
    N_faces = affected_faces.size();
    for (int i=0; i<N_faces; i++) {
        cur_face_id = affected_faces[i];
        face(cur_face_id).replacePoint(old_point_id, new_point_id, point_cache, vertex_cache);
        point_cache.removeFace(old_point_id, cur_face_id);
        point_cache.pairFace(new_point_id, cur_face_id);
    }
}

void EditMesh::replaceEdgeInFaces(Id old_edge_id, Id new_edge_id, IdSet& face_ids, bool invert) {
    std::vector<Id> affected_faces;

    // Collect affected faces
    Id   cur_face_id;
    bool in_set;
    int  N_faces = edge_cache.pairedFaceLen(old_edge_id);
    affected_faces.reserve(N_faces);
    for (int i=0; i<N_faces; i++) {
        cur_face_id = edgeToFace(old_edge_id, i);
        in_set      = face_ids.hasElement(cur_face_id);
        if ( in_set == !invert ) {
            affected_faces.push_back(cur_face_id);
        }
    }

    // Replace Edges in affected faces
    N_faces = affected_faces.size();
    for (int i=0; i<N_faces; i++) {
        cur_face_id = affected_faces[i];
        face(cur_face_id).replaceEdge(old_edge_id, new_edge_id);
        edge_cache.removeFace(old_edge_id, cur_face_id);
        edge_cache.pairFace(new_edge_id, cur_face_id);
    }
}

void EditMesh::replacePointsInEdge(IdSet& point_id_pairs, Id edge_id) {
    Id old_tail_id = edge(edge_id).pointId(0);
    Id old_tip_id  = edge(edge_id).pointId(1);

    Id new_point_id;
    if (point_id_pairs.hasElement(old_tail_id)) {
        new_point_id = point_id_pairs.value(old_tail_id);
        edge(edge_id).replacePoint(old_tail_id, new_point_id);
        point_cache.removeEdge(old_tail_id, edge_id);
        point_cache.pairEdge(new_point_id, edge_id);
    }

    if (point_id_pairs.hasElement(old_tip_id)) {
        new_point_id = point_id_pairs.value(old_tip_id);
        edge(edge_id).replacePoint(old_tip_id, new_point_id);
        point_cache.removeEdge(old_tip_id, edge_id);
        point_cache.pairEdge(new_point_id, edge_id);
    }
}

Id EditMesh::duplicateEdge(Id old_edge_id, IdSet& point_id_pairs) {
    // Creates a new edge from the old edge whose points are given as pairs.

    Id tail_id = edge(old_edge_id).pointId(0);
    Id tip_id  = edge(old_edge_id).pointId(1);

    if (point_id_pairs.hasElement(tail_id)) {
        tail_id = point_id_pairs.value(tail_id);
    }
    if (point_id_pairs.hasElement(tip_id)) {
        tip_id = point_id_pairs.value(tip_id);
    }

    return createEdge({tail_id, tip_id});
}

int EditMesh::classifyEdges(IdSet& edges, IdSet& faces_attached) {
    /* Returns the number of eSEEM edges in the set */
    Id cur_edge_id, cur_face_id;
    bool in_atch, n_in_atch;
    int N_edges, N_faces;
    int seem_num = 0;

    N_edges = edges.size();
    for (int i=0; i<N_edges; i++) {
        in_atch = false; n_in_atch = false;
        cur_edge_id = edges[i];
        N_faces = edge_cache.pairedFaceLen(cur_edge_id);
        for (int j=0; j<N_faces; j++) {
            cur_face_id = edgeToFace(cur_edge_id, j);
            if (faces_attached.hasElement(cur_face_id)) {
                in_atch = true;
            } else {
                n_in_atch = true;
            }
        }

        if (in_atch && n_in_atch) {
            edges.forceSetValue(emSEEM, i);
            seem_num += 1;
        } else if (in_atch) {
            edges.forceSetValue(emTOP, i);
        } else {
            edges.forceSetValue(emBOTTOM, i);
        }
    }
    return seem_num;
}