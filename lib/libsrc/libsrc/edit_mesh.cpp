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
EditMesh::EditMesh()
    :model_pos{ mat4::iden() } {} 

void EditMesh::translate(vec4 trans) {
    model_pos = mat4::translate(trans)*model_pos;
}

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
    Id new_tri_id = tri_cache.addTri(tri);
    new_face.addTri(new_tri_id);

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

    return new_face_id;
}

void EditMesh::load() {
    point_vbi.bindAllBuffers();
    point_vbi.loadVerticesStatic(point_cache.dataPtr(), point_cache.dataSize());
    point_vbi.configAttribf(0, 4, 4*sizeof(float), (void*)0);
    point_vbi.unbindCurrent();

    line_vbi.bindAllBuffers();
    line_vbi.loadVerticesStatic(point_cache.dataPtr(), point_cache.dataSize());
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

void EditMesh::draw(ShaderProgram& points, ShaderProgram& lines, ShaderProgram& faces, mat4 view) {
    int modelId;
    int viewId;
    int projID;

    const mat4 projMat = mat4::projPer(math::rads(45.0f), 800.0f/600.0f, 0.1f, 100.0f);

    modelId = faces.uniformLocation("model");
    viewId  = faces.uniformLocation("view");
    projID  = faces.uniformLocation("proj");
    faces.use();
    faces.uniformMat4f(modelId, model_pos);
    faces.uniformMat4f(viewId, view);
    faces.uniformMat4f(projID, projMat);

    face_vbi.bindCurrent();
    face_vbi.drawElementsStatic(tri_cache.indexLen());
    face_vbi.unbindCurrent();

    lines.use();
    modelId = lines.uniformLocation("model");
    viewId  = lines.uniformLocation("view");
    projID  = lines.uniformLocation("proj");
    lines.uniformMat4f(modelId, model_pos);
    lines.uniformMat4f(viewId, view);
    lines.uniformMat4f(projID, projMat);

    line_vbi.bindCurrent();
    glLineWidth(5);
    glDrawElements(GL_LINES, edge_cache.indexLen(), GL_UNSIGNED_INT, 0);
    line_vbi.unbindCurrent();

    points.use();
    modelId = points.uniformLocation("model");
    viewId  = points.uniformLocation("view");
    projID  = points.uniformLocation("proj");
    points.uniformMat4f(modelId, model_pos);
    points.uniformMat4f(viewId, view);
    points.uniformMat4f(projID, projMat);

    point_vbi.bindCurrent();
    point_vbi.drawPoints(point_cache.dataLen());
    point_vbi.unbindCurrent();
}