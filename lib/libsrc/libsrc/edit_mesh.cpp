#include "edit_mesh.hpp"

void EditFace::addPoint(unsigned int point_id)
    { points.push_back(point_id); }
void EditFace::addVertex(unsigned int vert_id)
    { vertices.push_back(vert_id); }
void EditFace::addEdge(unsigned int edge_id)
    { edges.push_back(edge_id); }
void EditFace::addTri(unsigned int tri_id)
    { tris.push_back(tri_id); }

// ======== EditMesh ========
Id EditMesh::createPoint(vec4 point)
    { return point_cache.addPoint(point); }

unsigned int EditMesh::createEdge(Id2 points) {
    Id new_edge = edge_cache.addEdge(EdgeIndexObj(points[0], points[1]));
    point_cache.pairEdge(points[0], new_edge);
    point_cache.pairEdge(points[1], new_edge);
    return new_edge;
}

Id EditMesh::createTri(Id3 points, Id3 edges) {
    Id3 tri_verts; 
    vec4 curr_pos;
    
    Id new_face_id = face_cache.createFace();
    EditFace& new_face = face_cache[new_face_id];

    for (int i=0; i<3; i++) {
        point_cache.pairFace(points[i], new_face_id);
        new_face.addPoint(points[i]);

        curr_pos = point_cache[points[i]].getPos();
        Id vert = vertex_cache.addVertex(curr_pos);
        tri_verts[i] = vert;

        edge_cache.pairFace(edges[i], new_face_id);
        new_face.addEdge(edges[i]);
    }

    TriIndexObj tri(tri_verts[0], tri_verts[1], tri_verts[2]);
    Id new_tri = tri_cache.addTri(tri);
    new_face.addTri(new_tri);

    return new_face_id;
} 

void EditMesh::load() {
    point_vbi.bindAllBuffers();
    point_vbi.loadVerticesStatic(point_cache.dataPtr(), point_cache.dataSize());
    point_vbi.configAttribf(0, 4, 4*sizeof(float), (void*)0);
    point_vbi.unbindCurrent();

    line_vbi.bindAllBuffers();
    line_vbi.loadVerticesStatic(point_cache.dataPtr(), point_cache.dataSize());

    unsigned int* ptr = (unsigned int*)edge_cache.dataPtr();
    line_vbi.loadIndicesStatic(edge_cache.dataPtr(), edge_cache.dataSize());
    line_vbi.configAttribf(0, 4, 4*sizeof(float), (void*)0);
    line_vbi.unbindCurrent();

    face_vbi.bindAllBuffers();
    face_vbi.loadVerticesStatic(vertex_cache.dataPtr(), vertex_cache.dataSize());
    face_vbi.loadIndicesStatic(tri_cache.dataPtr(), tri_cache.dataSize());
    face_vbi.configAttribf(0, 4, 10*sizeof(float), (void*)0);
    // vbi.configAttribf(1, 4, 10*sizeof(float), (void*)(4*sizeof(float)));
    // vbi.configAttribf(2, 2, 10*sizeof(float), (void*)(8*sizeof(float)));
    face_vbi.unbindCurrent();
}

void EditMesh::draw(ShaderProgram& points, ShaderProgram& lines, ShaderProgram& faces) {
    faces.use();
    face_vbi.bindCurrent();
    face_vbi.drawElementsStatic(tri_cache.indexLen());
    face_vbi.unbindCurrent();

    lines.use();
    line_vbi.bindCurrent();
    glLineWidth(5);
    glDrawElements(GL_LINES, edge_cache.indexLen(), GL_UNSIGNED_INT, 0);
    line_vbi.unbindCurrent();

    points.use();
    point_vbi.bindCurrent();
    point_vbi.drawPoints(point_cache.dataLen());
    point_vbi.unbindCurrent();
}