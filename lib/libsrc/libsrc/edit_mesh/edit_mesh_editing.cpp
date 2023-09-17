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
        curr_point_pos = point(point_ids[i]).getPos();

        // Create Vertex
        vert_ids[i] = vertex_cache.addVertex(curr_point_pos);

        // Pair Point Cache
        point_cache.pairVertex(point_ids[i], vert_ids[i]);
        point_cache.pairFace(point_ids[i], face_id);

        // Pair Vertex Cache
        vertex_cache.pairPoint(vert_ids[i], point_ids[i]);

        // Pair Edge Cache
        edge_cache.pairFace(edges_id[i], face_id);

        // Add to Face
        face(face_id).addPoint(point_ids[i], vert_ids[i]);
        face(face_id).addVertex(vert_ids[i]);
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
        curr_point_pos = point(point_ids[i]).getPos();

        // Create Vertex
        vert_ids[i] = vertex_cache.addVertex(curr_point_pos);

        // Pair Point Cache
        point_cache.pairVertex(point_ids[i], vert_ids[i]);
        point_cache.pairFace(point_ids[i], face_id);

        // Pair Vertex Cache
        vertex_cache.pairPoint(vert_ids[i], point_ids[i]);

        // Pair Edge Cache
        edge_cache.pairFace(edge_ids[i], face_id);

        // Add to Face
        face(face_id).addPoint(point_ids[i], vert_ids[i]);
        face(face_id).addVertex(vert_ids[i]);
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

void EditMesh::transformPoints(IdSet& point_ids, mat4 mat) {
    // Move and Reload Points and Vertices
    vec4 curr_pos;
    int N_points = point_ids.size();
    int M_verts;
    for (int i=0; i<N_points; i++) {
        curr_pos = point(point_ids[i]).getPos();
        curr_pos = mat*curr_pos;

        // Set and Reload Point
        point(point_ids[i]).setPos(curr_pos);
        reloadPoint(point_ids[i]);

        // Set Vertices
        M_verts = point_cache.pairedVertexLen(point_ids[i]);
        for (int j=0; j<M_verts; j++) {
            vertex(pointToVert(point_ids[i], j)).setPos(curr_pos);
        }
    }

    // Collect Affected Faces
    IdSet face_ids(face_cache.dataLen());
    int M_faces;
    for (int i=0; i<N_points; i++) {
        M_faces = point_cache.pairedFaceLen(point_ids[i]);
        for (int j=0; j<M_faces; j++) {
            face_ids.add(pointToFace(point_ids[i], j), point_ids[i]);
        }
    }

    // Recalculate and Reload Faces
    M_faces = face_ids.size();
    for (int i=0; i<M_faces; i++) {
        recalculateFaceCenter(face_ids[i]);
        recalculateFaceNormal(face_ids[i]);
        reloadFace(face_ids[i]);
    }
}

void EditMesh::transformPoints(mat4 mat) {
    transformPoints(selected_points, mat);
}

void EditMesh::recalculateFaceCenter(Id face_id) {
    vec4 center = face(face_id).calcCenter(point_cache);
    face(face_id).setCenter(center, vertex_cache);
}

void EditMesh::recalculateFaceNormal(Id face_id) {
    vec4 normal = face(face_id).calcNorm(tri_cache, vertex_cache);
    face(face_id).setNorm(normal, vertex_cache);
}

// === Testing ===
void EditMesh::replaceTest(Id old_point_id, Id top_face_id) {
    
}

void EditMesh::ripPointTest(Id old_point_id, IdSet& faces_attached) {
/*
    There are three types of edges, those that belong only to 
    the attatched faces, those that only to the bottom faces,
    and those are on the seem. The seem edges are split into
    two: top and bottom, these are created.
*/    

    // IdSet test_1(3);
    // IdSet test_3(4);

    // vec4 pos = point(old_point_id).getPos();
    // Id new_point_id = createPoint(pos);

    // replacePointInFaces(old_point_id, new_point_id, faces_attached, true);

    // int N_edges = point_cache.pairedEdgeLen(old_point_id);
    // IdSet edge_class(N_edges);
    // IdSet edge_map(N_edges);
    // for (int i=0; i<N_edges; i++) {
    //     edge_class.add(pointToEdge(old_point_id, i));
    // }
    // classifyEdges(edge_class, faces_attached);

    // Id new_edge_id, old_edge_id, other_point_id;
    // uint type;
    // for (int i=0; i<N_edges; i++) {
    //     old_edge_id = edge_class[i];
    //     type        = edge_class.forceGetValue(i);
    //     if (type == emSEEM) {
    //         other_point_id = edge(old_edge_id).otherId(old_point_id);
    //         new_edge_id    = createEdge({new_point_id, other_point_id});
    //         replaceEdgeInFaces(old_edge_id, new_edge_id, faces_attached, true);
    //         point_cache.removeEdge(old_point_id, old_edge_id);
    //     } else if (type == emBOTTOM) {
    //         edge(old_edge_id).replacePoint(old_edge_id, new_point_id);
    //         point_cache.removeEdge(old_point_id, old_edge_id);
    //     } else if (type == emTOP) {
    //         // Top
    //     }
    // }

    // load();

    // vec4 TRANS({0.0f, 0.5f, 0.0f, 1.0f});
    // IdSet points(1); points.add(old_point_id);
    // transformPoints(points, mat4::translate(TRANS));

    // Id atch_point_id = createPoint(att_pos);
    // Id bot_point_id = createPoint(pos);

    // replacePointInFaces(old_point_id, atch_point_id, faces_attached, false);
    // replacePointInFaces(old_point_id, bot_point_id, faces_attached, true);    

    // Id cur_edge_id;
    // int N_edges = point_cache.pairedEdgeLen(old_point_id);
    // IdSet edges(N_edges);
    // for (int i=0; i<N_edges; i++) {
    //     cur_edge_id = pointToEdge(old_point_id, i);
    //     edges.add(cur_edge_id);
    // }
    // classifyEdges(edges, faces_attached);

    // Id new_atch_edge_id;
    // Id new_bot_edge_id;
    // Id other_point_id;
    // uint type;
    // for (int i=0; i<N_edges; i++) {
    //     cur_edge_id = edges[i];
    //     type        = edges.forceGetValue(i);
    //     if ( type == emSEEM ) {
    //         other_point_id = edge(cur_edge_id).otherId(old_point_id);
    //         new_atch_edge_id = createEdge({atch_point_id, other_point_id});
    //         new_bot_edge_id  = createEdge({bot_point_id, other_point_id});
    //         replaceEdgeInFaces(old_point_id, new_atch_edge_id, faces_attached, false);
    //         replaceEdgeInFaces(old_point_id, new_bot_edge_id, faces_attached, true);
    //         edge_cache.depairEdge(cur_edge_id);
    //     } else if ( type == emTOP ) {
    //         edge(cur_edge_id).replacePoint(old_point_id, atch_point_id);
    //     } else if ( type == emBOTTOM ) {
    //         edge(cur_edge_id).replacePoint(old_point_id, bot_point_id);
    //     }
    // }
}

void EditMesh::replacePointInFaces(Id old_point_id, Id new_point_id, IdSet& face_ids, bool invert) {
    Id cur_face_id;
    bool in_set;
    int N_faces = point_cache.pairedFaceLen(old_point_id);
    for (int i=0; i<N_faces; i++) {
        cur_face_id = pointToFace(old_point_id, i);
        in_set      = face_ids.hasElement(cur_face_id);
        if ( in_set == !invert ) {
            face(cur_face_id).replacePoint(old_point_id, new_point_id, point_cache, vertex_cache);
            point_cache.removeFace(old_point_id, cur_face_id);
            point_cache.pairFace(new_point_id, cur_face_id);
        }
    }
}

void EditMesh::replaceEdgeInFaces(Id old_edge_id, Id new_edge_id, IdSet& face_ids, bool invert) {
    Id cur_face_id;
    bool in_set;
    int N_faces = edge_cache.pairedFaceLen(old_edge_id);
    for (int i=0; i<N_faces; i++) {
        cur_face_id = edgeToFace(old_edge_id, i);
        in_set      = face_ids.hasElement(cur_face_id);
        if ( in_set == !invert ) {
            face(cur_face_id).replaceEdge(old_edge_id, new_edge_id);
            edge_cache.removeFace(old_edge_id, cur_face_id);
            edge_cache.pairFace(new_edge_id, cur_face_id);
        }
    }
}

void EditMesh::classifyEdges(IdSet& edges, IdSet& faces_attached) {
    Id cur_edge_id, cur_face_id;
    int N_edges, N_faces;
    bool in_atch, n_in_atch;

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
        } else if (in_atch) {
            edges.forceSetValue(emTOP, i);
        } else {
            edges.forceSetValue(emBOTTOM, i);
        }

    }
}

void EditMesh::extrudeTest(Id face_id) {
    
}