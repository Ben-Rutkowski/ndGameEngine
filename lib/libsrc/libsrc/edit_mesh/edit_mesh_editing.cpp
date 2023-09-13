#include "edit_mesh/edit_mesh.hpp"

Id EditMesh::createPoint(vec4 point)
    { return point_cache.addPoint(point); }

unsigned int EditMesh::createEdge(Id2 points) {
    Id new_edge = edge_cache.addEdge(EdgeIndexObj(points[0], points[1]));
    point_cache.pairEdge(points[0], new_edge);
    point_cache.pairEdge(points[1], new_edge);
    return new_edge;
}

Id EditMesh::createTri(Id3 points, Id3 edges) {
    Id3  tri_verts; 
    vec4 curr_pos;
    
    Id new_face_id = face_cache.createFace();
    EditFace& new_face = face_cache[new_face_id];

    for (int i=0; i<3; i++) {
        point_cache.pairFace(points[i], new_face_id);
        new_face.addPoint(points[i]);

        curr_pos = point_cache[points[i]].getPos();
        Id vert = vertex_cache.addVertex(curr_pos);
        point_cache.pairVertex(points[i], vert);
        new_face.addVertex(vert);
        tri_verts[i] = vert;

        edge_cache.pairFace(edges[i], new_face_id);
        new_face.addEdge(edges[i]);
    }

    TriIndexObj tri(tri_verts[0], tri_verts[1], tri_verts[2]);
    Id new_tri_id = tri_cache.addTri(tri);
    new_face.addTri(new_tri_id);

    vec4 norm = new_face.calcNorm(tri_cache, vertex_cache);
    vec4 center = new_face.calcCenter(point_cache);
    new_face.setNorm(norm, vertex_cache);
    new_face.setCenter(center, vertex_cache);

    return new_face_id;
} 

Id EditMesh::createQuad(Id4 points, Id4 edges) {
    Id4 tri_verts;
    vec4 curr_pos;

    Id new_face_id = face_cache.createFace();
    EditFace& new_face = face_cache[new_face_id];

    for(int i=0; i<4; i++) {
        point_cache.pairFace(points[i], new_face_id);
        new_face.addPoint(points[i]);

        curr_pos = point_cache[points[i]].getPos();
        Id vert = vertex_cache.addVertex(curr_pos);
        point_cache.pairVertex(points[i], vert);
        new_face.addVertex(vert);
        tri_verts[i] = vert;

        edge_cache.pairFace(edges[i], new_face_id);
        new_face.addEdge(edges[i]);
    }

    TriIndexObj tri1(tri_verts[0], tri_verts[1], tri_verts[2]);
    TriIndexObj tri2(tri_verts[2], tri_verts[3], tri_verts[0]);
    Id new_tri_id1 = tri_cache.addTri(tri1);
    Id new_tri_id2 = tri_cache.addTri(tri2);
    new_face.addTri(new_tri_id1);
    new_face.addTri(new_tri_id2);

    vec4 norm = new_face.calcNorm(tri_cache, vertex_cache);
    vec4 center = new_face.calcCenter(point_cache);
    new_face.setNorm(norm, vertex_cache);
    new_face.setCenter(center, vertex_cache);

    return new_face_id;
}

void EditMesh::setPointPos(Id point, vec4 pos) {
    point_cache[point].setPos(pos);
    reloadPoint(point);

    Id vert_id;
    int N = point_cache.pairedVertexLen(point);
    for (int i=0; i<N; i++) {
        vert_id = point_cache.getPairedVertex(point, i);
        vertex_cache[vert_id].setPos(pos);
    }

    Id face_id;
    N = point_cache.pairedFaceLen(point);
    for (int i=0; i<N; i++) {
        face_id = point_cache.getPairedFace(point, i);
        face_cache[face_id].calcNorm(tri_cache, vertex_cache);
        face_cache[face_id].calcCenter(point_cache);
        reloadFace(face_id);
    }
}

void EditMesh::translatePoint(Id point, vec4 trans) {
    vec4 point_pos = point_cache[point].getPos();
    vec4 pos = vec4::sumK(point_pos, trans, 3);
    setPointPos(point, pos);
}

void EditMesh::translateSelectPoints(vec4 trans) {
    int N = select_points.size();
    for (int i=0; i<N; i++) {
        if (select_points[i]) {
            translatePoint(i, trans);
        }
    }
}

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