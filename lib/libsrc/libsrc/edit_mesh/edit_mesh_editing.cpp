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

    vec4 center = face(face_id).calcCenter(point_cache);
    vec4 normal = face(face_id).calcNorm(tri_cache, vertex_cache);
    face(face_id).setCenter(center, vertex_cache);
    face(face_id).setNorm(normal, vertex_cache);

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

    vec4 center = face(face_id).calcCenter(point_cache);
    vec4 normal = face(face_id).calcNorm(tri_cache, vertex_cache);
    face(face_id).setCenter(center, vertex_cache);
    face(face_id).setNorm(normal, vertex_cache);

    return face_id;
}

void EditMesh::transformPoints(std::vector<Id>& point_ids, mat4 mat) {
    // // Move and Reload Points and Vertices
    // vec4 curr_pos;
    // int N_points = point_ids.size();
    // int M_verts;
    // for (int i=0; i<N_points; i++) {
    //     curr_pos = point(point_ids[i]).getPos();
    //     curr_pos = mat*curr_pos;

    //     // Set and Reload Point
    //     point(point_ids[i]).setPos(curr_pos);
    //     reloadPoint(point_ids[i]);

    //     // Set Vertices
    //     M_verts = point_cache.pairedVertexLen(point_ids[i]);
    //     for (int j=0; j<M_verts; j++) {
    //         vertex(pointToVert(point_ids[i], j)).setPos(curr_pos);
    //     }
    // }

    // // Collect Affected Faces
    // UIntHashTable face_ids(face_cache.dataLen());
    // int M_faces;
    // for (int i=0; i<N_points; i++) {
    //     M_faces = point_cache.pairedFaceLen(point_ids[i]);
    //     for (int j=0; j<M_faces; j++) {
    //         face_ids.add(pointToFace(point_ids[i], j), point_ids[i]);
    //     }
    // }

    // // Recalculate and Reload Faces
    // M_faces = face_ids.size();
    // for (int i=0; i<M_faces; i++) {
    //     recalculateFace(face_ids[i]);
    //     reloadFace(face_ids[i]);
    // }
}

void EditMesh::transformPoints(mat4 mat) {
    // Move and Reload Points and Vertices
    vec4 curr_pos;
    int N_points = selected_points.size();
    int M_verts;
    for (int i=0; i<N_points; i++) {
        curr_pos = point(selected_points[i]).getPos();
        curr_pos = mat*curr_pos;

        // Set and Reload Point
        point(selected_points[i]).setPos(curr_pos);
        reloadPoint(selected_points[i]);

        // Set Vertices
        M_verts = point_cache.pairedVertexLen(selected_points[i]);
        for (int j=0; j<M_verts; j++) {
            vertex(pointToVert(selected_points[i], j)).setPos(curr_pos);
        }
    }

    // Collect Affected Faces
    UIntHashTable face_ids(face_cache.dataLen());
    int M_faces;
    for (int i=0; i<N_points; i++) {
        M_faces = point_cache.pairedFaceLen(selected_points[i]);
        for (int j=0; j<M_faces; j++) {
            face_ids.add(pointToFace(selected_points[i], j), selected_points[i]);
        }
    }

    // Recalculate and Reload Faces
    M_faces = face_ids.size();
    for (int i=0; i<M_faces; i++) {
        recalculateFace(face_ids[i]);
        reloadFace(face_ids[i]);
    }
}

void EditMesh::recalculateFace(Id face_id) {
    vec4 center = face(face_id).calcCenter(point_cache);
    vec4 normal = face(face_id).calcNorm(tri_cache, vertex_cache);
    face(face_id).setCenter(center, vertex_cache);
    face(face_id).setNorm(normal, vertex_cache);
}

// === Testing ===

void EditMesh::extrudeTest(Id face) {
    float AMOUNT = 0.1f;

    int pointN  = face_cache[face].pointLen();
    vec4 normal = face_cache[face].calcNorm(tri_cache, vertex_cache);
    vec4 displace = normal.multK(AMOUNT, 3);

    // Create the new points
    std::vector<Id> new_points(pointN);
    Id point_id;
    vec4 point_pos;
    for (int i=0; i<pointN; i++) {
        point_id = face_cache[face].pointId(i);
        point_pos = point_cache[point_id].getPos();
        point_pos = vec4::sumK(point_pos, displace, 3);
        new_points[i] = createPoint(point_pos);
    }
    std::vector<Id> new_edges(pointN);
    for (int i=0; i<pointN; i++) {
        new_edges[i] = createEdge({new_points[i%4], new_points[(i+1)%4]});
    }

    std::array<Id,4> points_quad {
        new_points[0], new_points[1], new_points[2], new_points[3]
    };
    std::array<Id,4> edges_quad = {
        new_edges[0], new_edges[1], new_edges[2], new_edges[3]
    };
    createQuad(points_quad, edges_quad);

    std::cout << "Point Number: " << point_cache.dataLen() << std::endl;

    load();
}