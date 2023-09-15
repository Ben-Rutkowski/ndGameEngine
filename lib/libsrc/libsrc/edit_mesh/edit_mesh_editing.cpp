#include "edit_mesh/edit_mesh.hpp"
#include "uint_hash_table.hpp"

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
    Id  face_id = face_cache.createFace();

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
        face(face_id).addPoint(point_ids[i]);
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
    Id  face_id = face_cache.createFace();

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
        face(face_id).addPoint(point_ids[i]);
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

void EditMesh::replacePointDoesntWork(Id old_point_id, Id new_point_id, IdSet& faces_attached) {
    // Create New Edges
    Id old_edge_id, new_edge_id;
    Id other_point_id;
    int N_edge = point_cache.pairedEdgeLen(old_point_id);
    IdSet edge_map(N_edge);
    for (int i=0; i<N_edge; i++) {
        old_edge_id    = pointToEdge(old_point_id, i);
        other_point_id = edge(old_edge_id).otherId(old_point_id);
        new_edge_id    = createEdge({other_point_id, new_point_id});
        point_cache.pairEdge(new_point_id, new_edge_id);
        edge_map.add(old_edge_id, new_edge_id);
    }

    // Collect Affected Faces, Replace Point and Pair Face
    Id cur_face_id;
    int N_face = point_cache.pairedFaceLen(old_point_id);
    IdSet aff_face_ids(N_face);
    for (int i=0; i<N_face; i++) {
        cur_face_id = point_cache.getPairedFace(old_point_id, i);
        if (!faces_attached.isElement(cur_face_id)) {
            face(cur_face_id).replacePoint(old_point_id, new_point_id, point_cache, vertex_cache);
            point_cache.pairFace(new_point_id, cur_face_id);
            aff_face_ids.add(cur_face_id, 0);
        }
    }

    // Replace and Pair New Edges in Affected Faces
    bool contains;
    N_edge = edge_map.size();
    N_face = aff_face_ids.size();
    for (int i=0; i<N_edge; i++) {              // i edge 
        old_edge_id = edge_map[i];
        new_edge_id = edge_map.getValue(old_edge_id);
        for (int j=0; j<N_face; j++) {          // j face
            contains = face(aff_face_ids[j]).replaceEdge(old_edge_id, new_edge_id);
            if (contains) {
                edge_cache.pairFace(new_edge_id, aff_face_ids[j]);
            }
        }
    }
}

void EditMesh::extrudeTest(Id face_id) {
    
}