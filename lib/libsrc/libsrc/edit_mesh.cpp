#include "edit_mesh.hpp"

void EditFace::addPoint(unsigned int point_id)
    { points.push_back(point_id); }
void EditFace::addVertex(unsigned int vert_id)
    { vertices.push_back(vert_id); }
void EditFace::addEdge(unsigned int edge_id)
    { edges.push_back(edge_id); }
void EditFace::addTri(unsigned int tri_id)
    { tris.push_back(tri_id); }

int EditFace::pointNum() { return points.size(); }
int EditFace::vertNum()  { return vertices.size(); }
int EditFace::edgeNum()  { return edges.size(); }
int EditFace::triNum()   { return tris.size(); }
Id EditFace::getPoint(int i) { return points[i]; }
Id EditFace::getVert(int i)  { return vertices[i]; }
Id EditFace::getEdge(int i)  { return edges[i]; } 
Id EditFace::getTri(int i)   { return tris[i]; } 

vec4 EditFace::calcNorm(TriCache& tri_cache, VertexCache& vertex_cache) {
    int N = triNum();
    vec4 curr_norm;
    vec4 norm = tri_cache[tris[0]].calcNorm(vertex_cache);
    for (int i=1; i<N; i++) {
        curr_norm = tri_cache[tris[0]].calcNorm(vertex_cache);
        norm = vec4::aveUnits4f(norm, curr_norm);
    }
    return norm;
}

void EditFace::setNorm(vec4 norm, VertexCache& vertex_cache) {
    int N = vertNum();
    for (int i=0; i<N; i++) {
        vertex_cache[vertices[i]].setNorm(norm);
    }
}

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
        new_face.addVertex(vert);
        tri_verts[i] = vert;

        edge_cache.pairFace(edges[i], new_face_id);
        new_face.addEdge(edges[i]);
    }

    TriIndexObj tri(tri_verts[0], tri_verts[1], tri_verts[2]);
    Id new_tri_id = tri_cache.addTri(tri);
    new_face.addTri(new_tri_id);

    vec4 norm = new_face.calcNorm(tri_cache, vertex_cache);
    new_face.setNorm(norm, vertex_cache);

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
    new_face.setNorm(norm, vertex_cache);

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
    face_vbi.configAttribf(1, 4, 10*sizeof(float), (void*)(4*sizeof(float)));
    // face_vbi.configAttribf(2, 2, 10*sizeof(float), (void*)(8*sizeof(float)));
    face_vbi.unbindCurrent();
}

void EditMesh::draw(ShaderProgram& points, ShaderProgram& lines, ShaderProgram& faces, mat4 view, mat4 proj, vec4 cfront) {
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    drawLines(lines, view, proj);
    drawPoints(points, view, proj);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);
    drawFaces(faces, view, proj, cfront);
    glDisable(GL_POLYGON_OFFSET_FILL);
}

void EditMesh::drawPoints(ShaderProgram& program, mat4 view, mat4 proj) {
    program.use();
    program.uniformMat4f("model", model_pos);
    program.uniformMat4f("view", view);
    program.uniformMat4f("proj", proj);

    point_vbi.bindCurrent();
    point_vbi.drawPoints(point_cache.dataLen());
    point_vbi.unbindCurrent();
}

void EditMesh::drawLines(ShaderProgram& program, mat4 view, mat4 proj) {
    program.use();
    program.uniformMat4f("model", model_pos);
    program.uniformMat4f("view", view);
    program.uniformMat4f("proj", proj);

    line_vbi.bindCurrent();
    glDrawElements(GL_LINES, edge_cache.indexLen(), GL_UNSIGNED_INT, 0);
    line_vbi.unbindCurrent();
    glDepthMask(GL_TRUE);
}

void EditMesh::drawFaces(ShaderProgram& program, mat4 view, mat4 proj, vec4 cfront) {
    program.use();
    program.uniformMat4f("model", model_pos);
    program.uniformMat4f("view", view);
    program.uniformMat4f("proj", proj);
    program.uniform4f("camera_front", cfront);

    face_vbi.bindCurrent();
    face_vbi.drawElementsStatic(tri_cache.indexLen());
    face_vbi.unbindCurrent();
}